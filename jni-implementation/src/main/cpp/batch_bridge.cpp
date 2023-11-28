#include "net_jllama_llama_cpp_java_bindings_LlamaContext_LlamaBatch.h"
#include "handler/LlamaManager.h"

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_00024LlamaBatch_submitSequenceNative
      (JNIEnv *env, jobject jBatch, jintArray jTokens, jint jSequenceId) {

  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_00024LlamaBatch_closeNative
      (JNIEnv *env, jobject jBatch) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaBatchFree(jBatch);
  }
}
