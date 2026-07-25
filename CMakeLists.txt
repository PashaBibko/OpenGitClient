cmake_minimum_required(VERSION 4.2.2)
project(OpenGitClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

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
        COMMAND npm run build -- --outDir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/web
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
add_library(InteropBinary SHARED IMPORTED)
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

# Creates the C++ executable
add_executable(OpenGitClient
        backend/main.cpp
        backend/InteropBindings/Functions.h
        backend/InteropBindings/OpenGitClientInterop.cpp
        backend/InteropBindings/OpenGitClientInterop.h
)

target_link_libraries(OpenGitClient PRIVATE InteropBinary)
add_dependencies(OpenGitClient ReactBuildProcess)

# O
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
