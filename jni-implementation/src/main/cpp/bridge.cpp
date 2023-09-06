#include <jni.h>
#include <unordered_map>
#include "net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl.h"
#include "exceptions/exceptions.h"
#include "handler/LlamaManager.h"
#include "libloader.h"

LlamaManager* llamaManager = nullptr;

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_loadLibrary(
      JNIEnv *env,
      jobject thisObject) {

    withJniExceptions(env, [env] {
      loadLibrary("llama");
      if (!llamaManager) {
        llamaManager = LlamaManager::getLlamaManager(env);
      }
    });
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_closeLibrary(
      JNIEnv *env,
      jobject thisObject) {
      withJniExceptions(env, [] {
        closeLibrary();
      });
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaBackendInit(
      JNIEnv *env,
      jobject thisObject,
      jboolean useNuma) {
    llamaManager->newSession(env).backendInit(useNuma);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaBackendFree(JNIEnv *env, jobject thisObject) {
    llamaManager->newSession(env).backendFree();
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLoadModelFromFile(
      JNIEnv *env, jobject thisObject, jbyteArray path, jobject javaParams) {
    return llamaManager->newSession(env).loadModelFromFile(path, javaParams);
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLoadContextWithModel
      (JNIEnv* env, jobject thisObject, jobject jModel, jobject jContextParams) {
    return llamaManager->newSession(env).loadContextWithModel(jModel, jContextParams);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenizeWithModel(JNIEnv* env, jobject thisObject, jobject jModel, jbyteArray jToTokenize, jintArray jTokensOut, jint jmaxTokens, jboolean jBos) {
    return llamaManager->newSession(env)
      .tokenizeWithModel(jModel, jToTokenize, jTokensOut, jmaxTokens, jBos);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaEval(
      JNIEnv* env,
      jobject thisObject,
      jobject jContext,
      jintArray jTokens,
      jint jnTokens,
      jint jnPast,
      jint jnThreads) {
    return llamaManager->newSession(env).eval(jContext, jTokens, jnTokens, jnPast, jnThreads);
  }

  JNIEXPORT jfloatArray JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaGetLogits(JNIEnv * env, jobject thisObject, jobject jContext) {
    return llamaManager->newSession(env).getLogits(jContext);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleToken(JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandiates) {
    return llamaManager->newSession(env).sampleToken(jContext, jCandiates);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleTokenGreedy (JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates) {
    return llamaManager->newSession(env).sampleTokenGreedy(jContext, jCandidates);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenToPiece
    (JNIEnv* env, jobject thisObject, jobject jContext, jint jToken, jbyteArray output) {
    return llamaManager->newSession(env).tokenToPiece(jContext, jToken, output);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenBos(JNIEnv* env, jobject thisObject, jobject jContext) {
    return llamaManager->newSession(env).tokenBos(jContext);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenEos(JNIEnv* env, jobject thisObject, jobject jContext) {
    return llamaManager->newSession(env).tokenEos(jContext);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenNl(JNIEnv* env, jobject thisObject, jobject jContext) {
    return llamaManager->newSession(env).tokenNl(jContext);
  }


  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLogSet(JNIEnv* env, jobject thisObject, jobject llamaLogCallback) {
    return llamaManager->newSession(env).setLogger(llamaLogCallback);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaFreeModel(JNIEnv* env, jobject thisObject, jobject jModel) {
    llamaManager->newSession(env).freeModel(jModel);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaFree
  (JNIEnv* env, jobject thisObject, jobject jContext) {
    llamaManager->newSession(env).freeContext(jContext);
  }

  JNIEXPORT jlong JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTimeUs(JNIEnv* env, jobject thisObject) {
    return llamaManager->newSession(env).getTimestampInMicroseconds();
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaContextDefaultParams(
      JNIEnv* env,
      jobject thisObject) {
    try {
      jobject ret = llamaManager->newSession(env).defaultContextParams();
      return ret;
    } catch (std::exception& e){
      return nullptr;
    }
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleRepetitionPenalty(JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates, jintArray lastTokens, jfloat penalty) {
    llamaManager->newSession(env).applyRepetitionPenalty(jContext, jCandidates, lastTokens, penalty);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleFrequencyAndPresencePenalties(JNIEnv * env, jobject thisObject, jobject jContext, jobject jCandidates, jintArray jLastTokens, jfloat jAlphaFrequency, jfloat jPenalty) {
    llamaManager->newSession(env).applyFrequencyAndPresencePenalties(jContext, jCandidates, jLastTokens, jAlphaFrequency, jPenalty);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleSoftMax
      (JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates) {
    llamaManager->newSession(env).llamaSampleSoftMax(jContext, jCandidates);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleTopK
      (JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates, jint k, jlong minKeep) {
    llamaManager->newSession(env).llamaSampleTopK(jContext, jCandidates, k, minKeep);
  }


}
