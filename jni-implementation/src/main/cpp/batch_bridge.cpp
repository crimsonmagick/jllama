#include "net_jllama_core_LlamaBatch.h"
#include "handler/LlamaManager.h"

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaBatch_llamaBatchFreeNative
      (JNIEnv* env, jobject jBatch) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaBatchFree(jBatch);
  }

}
