add_library(jni-implementation SHARED bridge.cpp jni.cpp libloader_unixlike.cpp libloader_windows.cpp handler/LlamaContextParamsManager.cpp Utf8StringManager.cpp handler/LlamaManager.cpp handler/LlamaSession.cpp jni.h exceptions/exceptions.h handler/LlamaManager.h)

target_include_directories(jni-implementation PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(jni-implementation ${JNI_LIBRARIES})

target_include_directories(jni-implementation PRIVATE ${JAVA_INCLUDE} ${JAVA_INCLUDE_OS} ${LLAMA_DIR})
target_include_directories(jni-implementation PUBLIC ${JNI_BINDINGS_DIR})
