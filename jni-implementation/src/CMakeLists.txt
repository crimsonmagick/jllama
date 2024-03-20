cmake_minimum_required(VERSION 3.22)
project(jni-implementation)

set(CMAKE_CXX_STANDARD 23)

find_package(JNI)

if (NOT JNI_FOUND)
    message(FATAL_ERROR "JNI not found. Stopping configuration...")
endif()

get_filename_component(JNI_BINDINGS_DIR ../../jni-bindings/build/generated/sources/headers/java/main ABSOLUTE)
get_filename_component(LLAMA_DIR external/llama.cpp ABSOLUTE)

option(BUILD_LLAMA_CPP "Option to build llama.cpp" OFF)

add_subdirectory(main/cpp)

if (BUILD_LLAMA_CPP)
    add_subdirectory(external/llama.cpp)
    set_target_properties(llama PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/llama"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/llama/debug"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/llama/release"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/llama"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/llama/debug"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/llama/release")
    add_dependencies(jni-implementation llama)
    if(MINGW)
        set_target_properties(llama PROPERTIES PREFIX "")
    endif()
endif()
