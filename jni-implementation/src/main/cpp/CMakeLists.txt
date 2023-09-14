add_library(jni-implementation SHARED bridge.cpp jni.cpp libloader_unixlike.cpp libloader_windows.cpp handler/LlamaContextParamsManager.cpp Utf8StringManager.cpp handler/LlamaManager.cpp handler/LlamaSession.cpp jni.h exceptions/exceptions.h handler/LlamaManager.h)

set_target_properties(jni-implementation PROPERTIES OUTPUT_NAME bridge PREFIX "")

target_include_directories(jni-implementation PRIVATE ${JAVA_INCLUDE} ${JAVA_INCLUDE_OS} ${LLAMA_DIR})
target_include_directories(jni-implementation PUBLIC ${JNI_BINDINGS_DIR})
