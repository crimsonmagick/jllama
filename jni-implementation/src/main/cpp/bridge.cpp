#include <jni.h>
#include <iostream>
#include "net_jllama_llama_cpp_java_bindings_LlamaCpp.h"
#include "exceptions/exceptions.h"
#include "handler/LlamaManager.h"
#include "libloader.h"

LlamaManager* llamaManager = nullptr;

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_loadLibrary(
      JNIEnv *env, jclass classReference) {

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
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_closeLibrary(
      JNIEnv *env,
      jclass classReference) {
      withJniExceptions(env, [] {
        closeLibrary();
      });
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaBackendInit(
      JNIEnv *env,
      jclass classReference,
      jboolean useNuma) {
    llamaManager->newSession(env).backendInit(useNuma);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaBackendFree(JNIEnv *env, jclass classReference) {
    llamaManager->newSession(env).backendFree();
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaLoadModelFromFile(
      JNIEnv *env, jclass classReference, jbyteArray path, jobject javaParams) {
    return llamaManager->newSession(env).loadModelFromFile(path, javaParams);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaTokenize(JNIEnv* env, jclass classReference, jobject jModel, jbyteArray jToTokenize, jintArray jTokensOut, jint jmaxTokens, jboolean jBos) {
    return llamaManager->newSession(env)
      .tokenizeWithModel(jModel, jToTokenize, jTokensOut, jmaxTokens, jBos);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaEval(
      JNIEnv* env,
      jclass classReference,
      jobject jContext,
      jintArray jTokens,
      jint jnTokens,
      jint jnPast) {
    return llamaManager->newSession(env).eval(jContext, jTokens, jnTokens, jnPast);
  }

  JNIEXPORT jfloatArray JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaGetLogits(JNIEnv * env, jclass classReference, jobject jContext) {
    return llamaManager->newSession(env).getLogits(jContext);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleToken(JNIEnv* env, jclass classReference, jobject jContext, jobject jCandiates) {
    return llamaManager->newSession(env).sampleToken(jContext, jCandiates);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleTokenGreedy (JNIEnv* env, jclass classReference, jobject jContext, jobject jCandidates) {
    return llamaManager->newSession(env).sampleTokenGreedy(jContext, jCandidates);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaTokenToPiece
    (JNIEnv* env, jclass classReference, jobject jModel, jint jToken, jbyteArray output) {
    return llamaManager->newSession(env).tokenToPiece(jModel, jToken, output);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaTokenBos(JNIEnv* env, jclass classReference, jobject jContext) {
    return llamaManager->newSession(env).tokenBos(jContext);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaTokenEos(JNIEnv* env, jclass classReference, jobject jContext) {
    return llamaManager->newSession(env).tokenEos(jContext);
  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaTokenNl(JNIEnv* env, jclass classReference, jobject jContext) {
    return llamaManager->newSession(env).tokenNl(jContext);
  }


  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaLogSet(JNIEnv* env, jclass classReference, jobject llamaLogCallback) {
    return llamaManager->newSession(env).setLogger(llamaLogCallback);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaFreeModel(JNIEnv* env, jclass classReference, jobject jModel) {
    llamaManager->newSession(env).freeModel(jModel);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaFree
  (JNIEnv* env, jclass classReference, jobject jContext) {
    llamaManager->newSession(env).freeContext(jContext);
  }

  JNIEXPORT jlong JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaTimeUs(JNIEnv* env, jclass classReference) {
    return llamaManager->newSession(env).getTimestampInMicroseconds();
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaContextDefaultParams(
      JNIEnv* env,
      jclass classReference) {
    try {
      jobject ret = llamaManager->newSession(env).defaultContextParams();
      return ret;
    } catch (std::exception& e){
      return nullptr;
    }
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaModelDefaultParams
      (JNIEnv *env, jclass classReference) {
    try {
      jobject ret = llamaManager->newSession(env).defaultModelParams();
      return ret;
    } catch (std::exception &e) {
      return nullptr;
    }
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleRepetitionPenalty(JNIEnv* env, jclass classReference, jobject jContext, jobject jCandidates, jintArray lastTokens, jfloat penalty) {
    llamaManager->newSession(env).applyRepetitionPenalty(jContext, jCandidates, lastTokens, penalty);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleFrequencyAndPresencePenalties(JNIEnv * env, jclass classReference, jobject jContext, jobject jCandidates, jintArray jLastTokens, jfloat jAlphaFrequency, jfloat jPenalty) {
    llamaManager->newSession(env).applyFrequencyAndPresencePenalties(jContext, jCandidates, jLastTokens, jAlphaFrequency, jPenalty);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleSoftMax
      (JNIEnv* env, jclass classReference, jobject jContext, jobject jCandidates) {
    llamaManager->newSession(env).llamaSampleSoftMax(jContext, jCandidates);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleTopK
      (JNIEnv* env, jclass classReference, jobject jContext, jobject jCandidates, jint k, jlong minKeep) {
    llamaManager->newSession(env).llamaSampleTopK(jContext, jCandidates, k, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleTopP(JNIEnv* env, jclass classReference, jobject jContext, jobject jCandidates, jfloat p, jlong minKeep) {
      llamaManager->newSession(env).llamaSampleTopP(jContext, jCandidates, p, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleTailFree(JNIEnv* env, jclass classReference, jobject jContext, jobject jCandidates, jfloat z, jlong minKeep) {
    llamaManager->newSession(env).llamaSampleTailFree(jContext, jCandidates, z, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleTypical(JNIEnv* env, jclass classReference, jobject jContext, jobject jCandidates, jfloat p, jint minKeep) {
    llamaManager->newSession(env).llamaSampleTypical(jContext, jCandidates, p, minKeep);
  }

  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaSampleTemperature(JNIEnv* env, jclass classReference, jobject jContext, jobject jCandidates, jfloat temp) {
    llamaManager->newSession(env).llamaSampleTemperature(jContext, jCandidates, temp);
  }

  JNIEXPORT jobject JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaBatchInit
    (JNIEnv* env, jclass classReference, jint nTokens, jint embd, jint nSeqMax) {
    return llamaManager->newSession(env).llamaBatchInit(nTokens, embd, nSeqMax);
  }


  JNIEXPORT void JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaBatchFree
      (JNIEnv* env, jclass classReference, jobject jBatch) {

  }

  JNIEXPORT jint JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaCpp_llamaDecode
      (JNIEnv* env, jclass classReference, jobject jContext, jobject jBatch) {
    return 0;
  }

}
