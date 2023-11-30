#include "net_jllama_llama_cpp_java_bindings_LlamaContext_LlamaBatch.h"
#include "handler/LlamaManager.h"

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_00024LlamaBatch_submitSequenceNative
      (JNIEnv *env, jobject jBatch, jintArray jTokens, jint jSequenceId, jint tokenSequenceIndex) {
    LlamaManager::getLlamaManager(env)->newSession(env)
      .submitSequence(jBatch, jTokens, jSequenceId, tokenSequenceIndex);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_00024LlamaBatch_closeNative
      (JNIEnv *env, jobject jBatch) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaBatchFree(jBatch);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_00024LlamaBatch_setCurrentTokenCountNative
      (JNIEnv* env, jobject jBatch, jint currentTokenCount) {
    LlamaManager::getLlamaManager(env)->newSession(env)
      .setCurrentTokenCount(jBatch, currentTokenCount);
  }
}
