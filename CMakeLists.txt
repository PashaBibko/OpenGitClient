cmake_minimum_required(VERSION 4.3)
project(OpenGitClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Configures variables for C# build depending on the environment
if(WIN32)
    message(FATAL_ERROR "Windows supported has not been configured yet.")
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

# Dummy file used to force rebuilds
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/_force_rebuild.txt "rebuild")
message(STATUS "Configured environment.")

# Builds the react-ts frontend
add_custom_target(ReactBuildProcess ALL
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/web
        COMMAND npm run build -- --outDir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/web
        COMMENT "Building React-TS frontend..."
        VERBATIM
)

# Builds the C# binary
add_custom_command(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Interop
        COMMAND dotnet publish -r ${CS_TARGET_OS} -c Release -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Building Interop C# binary..."
        OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_BINARY_NAME}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/_force_rebuild.txt
        VERBATIM
)

add_custom_target(InteropBuildProcess ALL
        DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_BINARY_NAME}
)

# Imports the compiled C# lib so it can be linked into the C++ build
add_library(InteropBinary SHARED IMPORTED)
set_target_properties(InteropBinary PROPERTIES
        IMPORTED_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${INTEROP_BINARY_NAME}
)
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

if(APPLE)
    target_link_libraries(OpenGitClient PRIVATE pthread)
endif()

# Makes sure the C# binary can be found by the native code
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

set_target_properties(OpenGitClient PROPERTIES
        BUILD_RPATH "@executable_path"
        INSTALL_RPATH "@executable_path"
)
