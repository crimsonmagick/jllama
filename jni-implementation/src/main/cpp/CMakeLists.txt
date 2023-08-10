add_library(jni_implementation SHARED bridge.cpp jni.cpp libloader_unixlike.cpp libloader_windows.cpp)

message(STATUS "JNI_BINDINGS_DIR: ${JNI_BINDINGS_DIR}")

target_include_directories(jni_implementation PRIVATE ${JAVA_INCLUDE} ${JAVA_INCLUDE_OS})
target_include_directories(jni_implementation PUBLIC ${JNI_BINDINGS_DIR})
