#include "net_jllama_core_LlamaContext_LlamaBatch.h"
#include "net_jllama_core_LlamaContext_LlamaBatchOld.h"
#include "handler/LlamaManager.h"

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_00024LlamaBatchOld_submitSequenceOldNative
      (JNIEnv *env, jobject jBatch, jintArray jTokens, jint jSequenceId, jint tokenSequenceIndex) {
    LlamaManager::getLlamaManager(env)->newSession(env)
      .submitSequence(jBatch, jTokens, jSequenceId, tokenSequenceIndex);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_00024LlamaBatchOld_closeNative
      (JNIEnv *env, jobject jBatch) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaBatchFree(jBatch);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_00024LlamaBatchOld_setCurrentTokenCountNative
      (JNIEnv* env, jobject jBatch, jint currentTokenCount) {
    LlamaManager::getLlamaManager(env)->newSession(env)
      .setCurrentTokenCount(jBatch, currentTokenCount);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_00024LlamaBatch_llamaBatchFreeNative
      (JNIEnv* env, jobject jBatch) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaBatchFree(jBatch);
  }

}
