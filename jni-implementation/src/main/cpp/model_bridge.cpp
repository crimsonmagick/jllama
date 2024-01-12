#include "net_jllama_core_LlamaModel.h"
#include "handler/LlamaManager.h"

extern "C" {

  JNIEXPORT jobject
  JNICALL Java_net_jllama_core_LlamaModel_createContextNative
      (JNIEnv* env, jobject jModel, jobject jContextParams) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).loadContextWithModel(jModel, jContextParams);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaModel_closeNative
      (JNIEnv* env, jobject jModel) {
    LlamaManager::getLlamaManager(env)->newSession(env).freeModel(jModel);
  }

  JNIEXPORT jint
  JNICALL Java_net_jllama_core_LlamaModel_llamaTokenizeNative
    (JNIEnv* env, jobject jModel, jbyteArray jToTokenize, jintArray jTokensOut, jint jmaxTokens, jboolean jBos, jboolean jSpecial) {
    return LlamaManager::getLlamaManager(env)->newSession(env)
    .tokenizeWithModel(jModel, jToTokenize, jTokensOut, jmaxTokens, jBos, jSpecial);
  }

  JNIEXPORT jint
  JNICALL Java_net_jllama_core_LlamaModel_detokenizeNative
      (JNIEnv* env, jobject jModel, jint jToken, jbyteArray output) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).tokenToPiece(jModel, jToken, output);
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_core_LlamaModel_llamaModelDefaultParams
      (JNIEnv *env, jclass classReference) {
    try {
      return LlamaManager::getLlamaManager(env)->newSession(env).defaultModelParams();
    } catch (std::exception &e) {
      return nullptr;
    }
  }


  JNIEXPORT jint
  JNICALL Java_net_jllama_core_LlamaModel_llamaTokenBosNative
      (JNIEnv *env, jobject jModel) {
    return LlamaManager::getLlamaManager(env)->newSession(env).tokenBos(jModel);
  }

  JNIEXPORT jint
  JNICALL Java_net_jllama_core_LlamaModel_llamaTokenEosNative(
      JNIEnv *env,
      jobject jModel) {
    return LlamaManager::getLlamaManager(env)->newSession(env).tokenEos(jModel);
  }

  JNIEXPORT jint
  JNICALL Java_net_jllama_core_LlamaModel_llamaTokenNlNative(
      JNIEnv *env,
      jobject jModel) {
    return LlamaManager::getLlamaManager(env)->newSession(env).tokenNl(jModel);
  }



}
