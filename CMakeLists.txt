cmake_minimum_required(VERSION 4.2.2)
project(OpenGitClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

include(FetchContent)

# Checks for compile time dependencies (npm and dotnet)
find_program(NPM_EXECUTABLE npm)
if(NOT NPM_EXECUTABLE)
    message(FATAL_ERROR "npm could not be found, make sure Node.js/npm is installed.")
endif()
message(STATUS "Found npm: ${NPM_EXECUTABLE}")

find_program(DOTNET_EXECUTABLE dotnet)
if(NOT DOTNET_EXECUTABLE)
    message(FATAL_ERROR "dotnet could not be found, make sure .NET SDK 8.0+ is installed.")
endif()
message(STATUS "Found dotnet: ${DOTNET_EXECUTABLE}")

# Configures the environment
if (WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Configures variables for C# build depending on the environment
if(WIN32)
    set(CS_TARGET_OS "win-x64")
    set(INTEROP_BINARY_NAME "Interop.lib")
    set(INTEROP_WINDOWS_DLL "Interop.dll")
elseif(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(CS_TARGET_OS "osx-arm64")
    else()
        set(CS_TARGET_OS "osx-x64")
    endif()
    set(INTEROP_BINARY_NAME "Interop.dylib")
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

# Builds the react-ts frontend
add_custom_target(NodeModulesInstall ALL
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/web
        COMMAND npm install
        COMMENT "Installing frontend dependencies."
        VERBATIM
)

add_custom_target(ReactBuildProcess ALL
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/web
        COMMAND npm run build -- --outDir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/web --emptyOutDir
        COMMENT "Building React-TS frontend."
        VERBATIM
)

add_dependencies(ReactBuildProcess NodeModulesInstall)

# Builds the C# binary
set(INTEROP_BUILD_COMMAND dotnet publish -r ${CS_TARGET_OS} -c Release -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(INTEROP_BUILD_COMMENT "Building C# interop library for [${CS_TARGET_OS}].")

if(WIN32)
    add_custom_target(InteropBuildProcess ALL
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Interop
            COMMAND ${INTEROP_BUILD_COMMAND}
            COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/Interop/bin/x64/Release/net8.0/win-x64/native/Interop.lib
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Interop.lib
            BYPRODUCTS
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_BINARY_NAME}
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_WINDOWS_DLL}
            COMMENT ${INTEROP_BUILD_COMMENT}
            VERBATIM
    )
elseif(APPLE)
    add_custom_target(InteropBuildProcess ALL
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Interop
            COMMAND ${INTEROP_BUILD_COMMAND}
            BYPRODUCTS
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_BINARY_NAME}
            COMMENT ${INTEROP_BUILD_COMMENT}
            VERBATIM
    )
else()
    # Should be covered before but another check incase above code changes
    message(FATAL_ERROR "Unsupported platform.")
endif()

# Imports the compiled C# lib so it can be linked into the C++ build
add_library(InteropBinary SHARED IMPORTED
        backend/Functions/RepoLocation.cpp)
if(WIN32)
    set_target_properties(InteropBinary PROPERTIES
            IMPORTED_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_WINDOWS_DLL}
            IMPORTED_IMPLIB   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_BINARY_NAME}
    )
elseif(APPLE)
    set_target_properties(InteropBinary PROPERTIES
            IMPORTED_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_BINARY_NAME}
    )
else()
    # Should be covered before but another check incase above code changes
    message(FATAL_ERROR "Unsupported platform.")
endif()

add_dependencies(InteropBinary InteropBuildProcess)

# Imports the C++ dependencies
FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG v7.9.1
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Glaze)

set(STATIC_CRT OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        libgit2
        GIT_REPOSITORY https://github.com/libgit2/libgit2.git
        GIT_TAG        v1.9.6
        GIT_SHALLOW    TRUE
)

# Trims part of libgit's build process to speed it up
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CLI OFF CACHE BOOL "" FORCE)
set(USE_SSH OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(libgit2)

FetchContent_Declare(
        nfd
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
        GIT_TAG        v1.3.0
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nfd)

# Creates the C++ executable from the project source files
add_executable(OpenGitClient
        backend/InteropBindings/Functions.h
        backend/InteropBindings/OpenGitClientInterop.cpp
        backend/InteropBindings/OpenGitClientInterop.h
        backend/Router/WebFunctionRouter.h
        backend/Router/WebFunction.h
        backend/Functions/GetRepoChanges.cpp
        backend/Functions/ChooseOrOpenRepo.cpp
        backend/Functions/GetFileDiff.cpp
        backend/Logger/LoggerWebFunctions.h
        backend/Logger/Logger.h
        backend/Functions.h
        backend/AppContext.h
        backend/UserData.h
        backend/UserData.cpp
        backend/main.cpp
)

# Links the C++ executable to the included libraries + the C# interop binary
target_link_libraries(OpenGitClient PRIVATE InteropBinary)
target_link_libraries(OpenGitClient PRIVATE glaze::glaze)
target_link_libraries(OpenGitClient PRIVATE libgit2package)
target_link_libraries(OpenGitClient PRIVATE nfd)

target_include_directories(OpenGitClient PRIVATE ${libgit2_SOURCE_DIR}/include)

# Makes sure that the react frontend is always recompiled even though at the compile time stage it is
# not directly needed it just avoids pointless errors of using an outdated version of the frontend by accident
add_dependencies(OpenGitClient ReactBuildProcess)

# Due to how .dylib works on apple systems it requires additional setup so the
# final binaries are able to locate each other to access the functions declared in them
if(APPLE)
    target_link_libraries(OpenGitClient PRIVATE pthread)
    add_custom_command(TARGET InteropBuildProcess POST_BUILD
            COMMAND install_name_tool -id @rpath/${INTEROP_BINARY_NAME}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_BINARY_NAME}
    )

    add_custom_command(TARGET OpenGitClient POST_BUILD
            COMMAND install_name_tool -change
            bin/Release/net8.0/${CS_TARGET_OS}/native/${INTEROP_BINARY_NAME}
            @rpath/${INTEROP_BINARY_NAME}
            $<TARGET_FILE:OpenGitClient>
    )

    # Makes sure the .exe can locate the
    set_target_properties(OpenGitClient PROPERTIES
            BUILD_RPATH "@executable_path"
            INSTALL_RPATH "@executable_path"
    )
endif()
