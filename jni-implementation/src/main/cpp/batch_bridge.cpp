#include "net_jllama_core_LlamaContext_LlamaBatch.h"
#include "handler/LlamaManager.h"

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_00024LlamaBatch_llamaBatchFreeNative
      (JNIEnv* env, jobject jBatch) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaBatchFree(jBatch);
  }

}
