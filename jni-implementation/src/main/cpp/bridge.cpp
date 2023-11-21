#include <jni.h>
#include <iostream>
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
      const char* libraryName = "llama";
      try {
        loadLibrary(libraryName);
      } catch (const DynamicLibraryException& e) {
        std::cout << "Unable to load library based on environment variables. Attempting to load from Java resources" << std::endl;
        try {
          loadLibrary(libraryName, env);
        } catch (const DynamicLibraryException& e) {
          std::cout << "Unable to load library from Java resources" << std::endl;
          jni::throwDLLException(env, e);
        }
      }
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
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaNewContextWithModel
      (JNIEnv* env, jobject thisObject, jobject jModel, jobject jContextParams) {
    return llamaManager->newSession(env).loadContextWithModel(jModel, jContextParams);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenize(JNIEnv* env, jobject thisObject, jobject jModel, jbyteArray jToTokenize, jintArray jTokensOut, jint jmaxTokens, jboolean jBos) {
    return llamaManager->newSession(env)
      .tokenizeWithModel(jModel, jToTokenize, jTokensOut, jmaxTokens, jBos);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaEval(
      JNIEnv* env,
      jobject thisObject,
      jobject jContext,
      jintArray jTokens,
      jint jnTokens,
      jint jnPast) {
    return llamaManager->newSession(env).eval(jContext, jTokens, jnTokens, jnPast);
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
    (JNIEnv* env, jobject thisObject, jobject jModel, jint jToken, jbyteArray output) {
    return llamaManager->newSession(env).tokenToPiece(jModel, jToken, output);
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

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaModelDefaultParams
      (JNIEnv *env, jobject thisObject) {
    try {
      jobject ret = llamaManager->newSession(env).defaultModelParams();
      return ret;
    } catch (std::exception &e) {
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

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleTopP(JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates, jfloat p, jlong minKeep) {
      llamaManager->newSession(env).llamaSampleTopP(jContext, jCandidates, p, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleTailFree(JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates, jfloat z, jlong minKeep) {
    llamaManager->newSession(env).llamaSampleTailFree(jContext, jCandidates, z, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleTypical(JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates, jfloat p, jint minKeep) {
    llamaManager->newSession(env).llamaSampleTypical(jContext, jCandidates, p, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaSampleTemperature(JNIEnv* env, jobject thisObject, jobject jContext, jobject jCandidates, jfloat temp) {
    llamaManager->newSession(env).llamaSampleTemperature(jContext, jCandidates, temp);
  }

  JNIEXPORT jobject JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaBatchInit
    (JNIEnv* env, jobject thisObject, jint nTokens, jint embd, jint nSeqMax) {
    return llamaManager->newSession(env).llamaBatchInit(nTokens, embd, nSeqMax);
  }


  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaBatchFree
      (JNIEnv* env, jobject thisObject, jobject jBatch) {

  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaDecode
      (JNIEnv* env, jobject thisObject, jobject jContext, jobject jBatch) {
    return 0;
  }

}
