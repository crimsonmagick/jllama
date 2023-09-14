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
endif()
