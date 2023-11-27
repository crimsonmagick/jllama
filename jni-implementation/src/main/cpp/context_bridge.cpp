#include "net_jllama_llama_cpp_java_bindings_LlamaContext.h"
#include "handler/LlamaManager.h"

extern "C" {
  JNIEXPORT jint
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaEvalNative
      (JNIEnv* env,
       jobject jContext,
       jintArray jTokens,
       jint jnTokens,
       jint jnPast) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).eval(jContext, jTokens, jnTokens, jnPast);
  }

  JNIEXPORT jfloatArray JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaGetLogitsNative(JNIEnv * env, jobject jContext) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).getLogits(jContext);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_closeNative
      (JNIEnv* env, jobject jContext) {
    LlamaManager::getLlamaManager(env)->newSession(env).freeContext(jContext);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleSoftMaxNative
      (JNIEnv* env, jobject jContext, jobject jCandidates) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleSoftMax(jContext, jCandidates);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleTopKNative
      (JNIEnv* env, jobject jContext, jobject jCandidates, jint k, jlong minKeep) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTopK(jContext, jCandidates, k, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleTopPNative(JNIEnv* env, jobject jContext, jobject jCandidates, jfloat p, jlong minKeep) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTopP(jContext, jCandidates, p, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleTailFreeNative(JNIEnv* env, jobject jContext, jobject jCandidates, jfloat z, jlong minKeep) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTailFree(jContext, jCandidates, z, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleTypicalNative(JNIEnv* env, jobject jContext, jobject jCandidates, jfloat p, jint minKeep) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTypical(jContext, jCandidates, p, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleTemperatureNative(JNIEnv* env, jobject jContext, jobject jCandidates, jfloat temp) {
    LlamaManager::getLlamaManager(env)->newSession(env).llamaSampleTemperature(jContext, jCandidates, temp);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleRepetitionPenaltyNative(JNIEnv* env, jobject jContext, jobject jCandidates, jintArray lastTokens, jfloat penalty) {
    LlamaManager::getLlamaManager(env)->newSession(env).applyRepetitionPenalty(jContext, jCandidates, lastTokens, penalty);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleFrequencyAndPresencePenaltiesNative(JNIEnv * env, jobject jContext, jobject jCandidates, jintArray jLastTokens, jfloat jAlphaFrequency, jfloat jPenalty) {
    LlamaManager::getLlamaManager(env)->newSession(env).applyFrequencyAndPresencePenalties(jContext, jCandidates, jLastTokens, jAlphaFrequency, jPenalty);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaTokenBosNative(JNIEnv* env, jobject jContext) {
    return LlamaManager::getLlamaManager(env)->newSession(env).tokenBos(jContext);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaTokenEosNative(JNIEnv* env, jobject jContext) {
    return LlamaManager::getLlamaManager(env)->newSession(env).tokenEos(jContext);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaTokenNlNative(JNIEnv* env, jobject jContext) {
    return LlamaManager::getLlamaManager(env)->newSession(env).tokenNl(jContext);
  }


  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleTokenNative(JNIEnv* env, jobject jContext, jobject jCandiates) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).sampleToken(jContext, jCandiates);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaSampleTokenGreedyNative(JNIEnv* env, jobject jContext, jobject jCandidates) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).sampleTokenGreedy(jContext, jCandidates);
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaContextDefaultParams(JNIEnv* env,jclass classReference) {

    try {
      jobject ret = LlamaManager::getLlamaManager(env)->newSession(env).defaultContextParams();
      return ret;
    } catch (std::exception& e) {
      return nullptr;
    }
  }

}
