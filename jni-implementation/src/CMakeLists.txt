cmake_minimum_required(VERSION 3.22)
project(jni-implementation)

set(CMAKE_CXX_STANDARD 23)

get_filename_component(JNI_BINDINGS_DIR ../../jni-bindings/build/generated/sources/headers/java/main ABSOLUTE)
get_filename_component(LLAMA_DIR external/llama.cpp ABSOLUTE)

set(JAVA_HOME $ENV{JAVA_HOME})
set(JAVA_INCLUDE ${JAVA_HOME}/include)
set (JAVA_INCLUDE_OS )

if(WIN32) # True for both 32-bit and 64-bit Windows
    set(JAVA_INCLUDE_OS ${JAVA_INCLUDE}/win32)
elseif(APPLE)
    set(JAVA_INCLUDE_OS ${JAVA_INCLUDE}/darwin)
elseif(UNIX) # True for UNIX-like OS's, including Linux and BSD
    set(JAVA_INCLUDE_OS ${JAVA_INCLUDE}/linux)
else()
    message(FATAL_ERROR "Unrecognized Operating System. Stopping configuration...")
endif()

add_subdirectory(main/cpp)

option(BUILD_LLAMA_CPP "Option to build llama.cpp" OFF)

if (BUILD_LLAMA_CPP)
    add_subdirectory(external/llama.cpp)
endif()

