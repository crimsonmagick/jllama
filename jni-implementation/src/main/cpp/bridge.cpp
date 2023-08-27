#include <jni.h>
#include <unordered_map>
#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl.h"
#include "handler/LlamaSession.h"
#include "libloader.h"
#include "handler/LlamaManager.h"
#include "exceptions/exceptions.h"

LlamaManager* llamaManager = nullptr;

extern "C" {

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_loadLibrary(
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
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_closeLibrary(
      JNIEnv *env,
      jobject thisObject) {
      withJniExceptions(env, [] {
        closeLibrary();
      });
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaBackendInit(
      JNIEnv *env,
      jobject thisObject,
      jboolean useNuma) {
    LlamaSession(env).backendInit(useNuma);
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaBackendFree(JNIEnv *env, jobject thisObject) {
    LlamaSession(env).backendFree();
  }

  JNIEXPORT jobject
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLoadModelFromFile(
      JNIEnv *env, jobject thisObject, jbyteArray path, jobject javaParams) {
    return LlamaSession(env).loadModelFromFile(path, javaParams);
  }

  JNIEXPORT jobject
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLoadContextWithModel
      (JNIEnv* env, jobject thisObject, jobject jModel, jobject jContextParams) {
    return LlamaSession(env).loadContextWithModel(jModel, jContextParams);
  }

  JNIEXPORT jint
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenizeWithModel
      (JNIEnv* env, jobject thisObject, jobject jModel, jbyteArray jToTokenize, jintArray jTokensOut, jint jmaxTokens,
       jboolean jBos) {
    return LlamaSession(env).tokenizeWithModel(jModel, jToTokenize, jTokensOut, jmaxTokens, jBos);
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaEval(
      JNIEnv* env,
      jobject thisObject,
      jobject jContext,
      jintArray jTokens,
      jint jnTokens,
      jint jnPast,
      jint jnThreads) {
    return LlamaSession(env).eval(jContext, jTokens, jnTokens, jnPast, jnThreads);
  }

  JNIEXPORT jfloatArray JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaGetLogits(JNIEnv * env, jobject thisObject, jobject jContext) {
    return LlamaSession(env).getLogits(jContext);
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleTokenGreedy (JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates) {
    return LlamaSession(env).sampleTokenGreedy(jContext, jCandidates);
  }

  JNIEXPORT jbyteArray JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenToStr(JNIEnv* env, jobject thisObject, jobject jContext, jint jToken) {
    return LlamaSession(env).tokenToStr(jContext, jToken);
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenBos(JNIEnv* env, jobject thisObject) {
    return LlamaSession(env).tokenBos();
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenEos(JNIEnv* env, jobject thisObject) {
    return LlamaSession(env).tokenEos();
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenNl(JNIEnv* env, jobject thisObject) {
    return LlamaSession(env).tokenNl();
  }

}
