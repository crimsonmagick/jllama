#include "net_jllama_core_LlamaContext.h"
#include "handler/LlamaManager.h"

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_closeNative
      (JNIEnv* env, jobject jContext) {
    LlamaManager::getLlamaManager(env)->newSession(env).freeContext(jContext);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaSampleSoftmaxNative(JNIEnv* jniEnv, jobject jContext, jobject jCandidates) {
    LlamaManager::getLlamaManager(jniEnv)->newSession(jniEnv).llamaSampleSoftmax(jContext, jCandidates);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaSampleTopKNative
    (JNIEnv* env, jobject jContext, jobject jCandidates, jint k, jlong minKeep) {
      LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTopK(jContext, jCandidates, k, minKeep);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaSampleTopPNative
    (JNIEnv* env, jobject jContext, jobject jCandidates, jfloat p, jlong minKeep) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTopP(jContext, jCandidates, p, minKeep);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaSampleMinPNative
    (JNIEnv* env, jobject jContext, jobject jCandidates, jfloat jP, jlong jMinKeep) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleMinP(jContext, jCandidates, jP, jMinKeep);
  }


JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaSampleTailFreeNative(JNIEnv* env, jobject jContext, jobject jCandidates, jfloat z, jlong minKeep) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTailFree(jContext, jCandidates, z, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_core_LlamaContext_llamaSampleTypicalNative(JNIEnv* env, jobject jContext, jobject jCandidates, jfloat p, jint minKeep) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTypical(jContext, jCandidates, p, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_core_LlamaContext_llamaSampleTempNative(JNIEnv* env, jobject jContext, jobject jCandidates, jfloat temp) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTemperature(jContext, jCandidates, temp);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_core_LlamaContext_llamaSampleTokenNative(JNIEnv* env, jobject jContext, jobject jCandiates) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).sampleToken(jContext, jCandiates);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_core_LlamaContext_llamaSampleTokenGreedyNative(JNIEnv* env, jobject jContext, jobject jCandidates) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).sampleTokenGreedy(jContext, jCandidates);
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_core_LlamaContext_llamaBatchInitNative
      (JNIEnv* env, jobject jContext, jint nTokens, jint embd, jint nSeqId
      ) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).llamaBatchInit(jContext, nTokens, embd, nSeqId);
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_core_LlamaContext_llamaContextDefaultParams(JNIEnv* env, jclass classReference) {

    try {
      return LlamaManager::getLlamaManager(env)->newSession(env).defaultContextParams();
    } catch (std::exception& e) {
      return nullptr;
    }
  }

  JNIEXPORT jfloatArray
  JNICALL Java_net_jllama_core_LlamaContext_getLogitsNative
    (JNIEnv* env, jobject jContext, jint batchTokenIndex) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).getLogitsIth(jContext, batchTokenIndex);
  }

  JNIEXPORT jint
  JNICALL Java_net_jllama_core_LlamaContext_llamaDecodeNative
    (JNIEnv* env, jobject jContext, jobject jBatch) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).decodeNative(jContext, jBatch);
  }

  JNIEXPORT jfloatArray
  JNICALL
  Java_net_jllama_core_LlamaContext_llamaGetLogitsIthNative
      (JNIEnv* env, jobject jContext, jint i) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).getLogitsIth(jContext, i);
  }

  JNIEXPORT jint
  JNICALL Java_net_jllama_core_LlamaContext_llamaGetKvCacheUsedCellsNative
      (JNIEnv* env, jobject jContext) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).getKvCacheUsedCells(jContext);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaKvCacheClearNative
    (JNIEnv* env, jobject jContext) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).kvCacheClear(jContext);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaKvCacheSeqRmNative
    (JNIEnv* jEnv, jobject jContext, jint seqId, jint p0, jint p1) {
    return LlamaManager::getLlamaManager(jEnv)
        ->newSession(jEnv).kvCacheSeqRm(jContext, seqId, p0, p1);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaKvCacheSeqCpNative
    (JNIEnv* env, jobject jContext, jint seqId, jint seqIdDst, jint p0, jint p1) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).kvCacheSeqCp(jContext, seqId, seqIdDst, p0, p1);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaKvCacheSeqKeepNative
    (JNIEnv* env, jobject jContext, jint seqId) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).kvCacheSeqKeep(jContext, seqId);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaContext_llamaKvCacheSeqShiftNative(JNIEnv* env, jobject jContext, jint seqId, jint p0, jint p1, jint delta) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).kvCacheSeqShift(jContext, seqId, p0, p1, delta);
  }

}
