add_library(jni-implementation SHARED bridge.cpp jni.cpp libloader_unixlike.cpp libloader_windows.cpp)

target_include_directories(jni-implementation PRIVATE ${JAVA_INCLUDE} ${JAVA_INCLUDE_OS} ${LLAMA_DIR})
target_include_directories(jni-implementation PUBLIC ${JNI_BINDINGS_DIR})
