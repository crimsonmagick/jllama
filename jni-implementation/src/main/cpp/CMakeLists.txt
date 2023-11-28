add_library(jni-implementation SHARED
        bridge.cpp jni.cpp libloader_unixlike.cpp libloader_windows.cpp
        handler/LlamaContextParamsManager.cpp Utf8StringManager.cpp handler/LlamaManager.cpp
        handler/LlamaSession.cpp jni.h exceptions/exceptions.h handler/LlamaManager.h
        handler/LlamaModelParamsManager.cpp
        model_bridge.cpp
        context_bridge.cpp batch_bridge.cpp)

set_target_properties(jni-implementation PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/jni-implementation"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/jni-implementation/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/jni-implementation/release"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/jni-implementation"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/jni-implementation/debug"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/jni-implementation/release")

if (USING_MINGW_TOOLCHAIN)
    set_target_properties(jni-implementation PROPERTIES PREFIX "")
endif ()

target_include_directories(jni-implementation PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(jni-implementation ${JNI_LIBRARIES})

target_include_directories(jni-implementation PRIVATE ${JAVA_INCLUDE} ${JAVA_INCLUDE_OS} ${LLAMA_DIR})
target_include_directories(jni-implementation PUBLIC ${JNI_BINDINGS_DIR})
