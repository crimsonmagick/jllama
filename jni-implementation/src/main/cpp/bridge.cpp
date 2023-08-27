#include <jni.h>
#include <unordered_map>
#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl.h"
#include "exceptions/exceptions.h"
#include "handler/LlamaManager.h"
#include "libloader.h"

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
    llamaManager->newSession(env).backendInit(useNuma);
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaBackendFree(JNIEnv *env, jobject thisObject) {
    llamaManager->newSession(env).backendFree();
  }

  JNIEXPORT jobject
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLoadModelFromFile(
      JNIEnv *env, jobject thisObject, jbyteArray path, jobject javaParams) {
    return llamaManager->newSession(env).loadModelFromFile(path, javaParams);
  }

  JNIEXPORT jobject
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLoadContextWithModel
      (JNIEnv* env, jobject thisObject, jobject jModel, jobject jContextParams) {
    return llamaManager->newSession(env).loadContextWithModel(jModel, jContextParams);
  }

[[maybe_unused]] JNIEXPORT jint
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenizeWithModel
      (JNIEnv* env, jobject thisObject, jobject jModel, jbyteArray jToTokenize, jintArray jTokensOut, jint jmaxTokens,
       jboolean jBos) {
  return llamaManager->newSession(env)
      .tokenizeWithModel(jModel, jToTokenize, jTokensOut, jmaxTokens, jBos);
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaEval(
      JNIEnv* env,
      jobject thisObject,
      jobject jContext,
      jintArray jTokens,
      jint jnTokens,
      jint jnPast,
      jint jnThreads) {
    return llamaManager->newSession(env).eval(jContext, jTokens, jnTokens, jnPast, jnThreads);
  }

  JNIEXPORT jfloatArray JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaGetLogits(JNIEnv * env, jobject thisObject, jobject jContext) {
    return llamaManager->newSession(env).getLogits(jContext);
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleTokenGreedy (JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates) {
    return llamaManager->newSession(env).sampleTokenGreedy(jContext, jCandidates);
  }

  JNIEXPORT jbyteArray JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenToStr(JNIEnv* env, jobject thisObject, jobject jContext, jint jToken) {
    return llamaManager->newSession(env).tokenToStr(jContext, jToken);
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenBos(JNIEnv* env, jobject thisObject) {
    return llamaManager->newSession(env).tokenBos();
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenEos(JNIEnv* env, jobject thisObject) {
    return llamaManager->newSession(env).tokenEos();
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenNl(JNIEnv* env, jobject thisObject) {
    return llamaManager->newSession(env).tokenNl();
  }

}
