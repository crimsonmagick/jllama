#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl.h"
#include "exceptions/DynamicLibraryException.h"
#include "jni.h"
#include "libloader.h"
#include "llama.h"
#include "LlamaContextParamsManager.h"
#include "Utf8StringManager.h"
#include "handler/LlamaSession.h"

const int FAILURE = 1;

typedef const char* (* llama_token_to_str_pointer)(llama_context*, llama_token);
typedef llama_token (* llama_sample_token_greedy_pointer)
    (struct llama_context*, llama_token_data_array*);
typedef llama_token (* get_special_token_pointer)();

extern "C" {

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_loadLibrary(
      JNIEnv *env,
      jobject thisObject) {

    try {
      loadLibrary("llama");
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_closeLibrary(
      JNIEnv *env,
      jobject thisObject) {
    try {
      closeLibrary();
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
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
    try {
      auto sampleTokenGreedily = (llama_sample_token_greedy_pointer) getFunctionAddress("llama_sample_token_greedy");
      auto llamaContext = jni::getLlamaContextPointer(env, jContext);
      llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
      jint sampled = sampleTokenGreedily(llamaContext, &candidates);
      delete[] candidates.data;
      return sampled;
    } catch (const DynamicLibraryException& e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException& e) {
      jni::throwJNIException(env, e);
    }
    return FAILURE;
  }

  JNIEXPORT jbyteArray JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenToStr(JNIEnv* env, jobject thisObject, jobject jContext, jint jToken) {
    try {
      auto detokenize = (llama_token_to_str_pointer) getFunctionAddress(
          "llama_token_to_str");

      auto llamaContext = jni::getLlamaContextPointer(env, jContext);

      const char * result = detokenize(llamaContext, jToken);
      auto length = static_cast<jsize>(strlen(result));
      jbyteArray detokenized = env->NewByteArray(length);
      env->SetByteArrayRegion(detokenized, 0, length, reinterpret_cast<const jbyte*>(result));
      return detokenized;
    } catch (const DynamicLibraryException& e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException& e) {
      jni::throwJNIException(env, e);
    }
    return nullptr;
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenBos(JNIEnv* env, jobject thisObject) {
    try {
      auto getBos = (get_special_token_pointer) getFunctionAddress("llama_token_bos");
      return getBos();
    } catch (const DynamicLibraryException& e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException& e) {
      jni::throwJNIException(env, e);
    }
    return FAILURE;
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenEos(JNIEnv* env, jobject thisObject) {
    try {
      auto getEos = (get_special_token_pointer) getFunctionAddress("llama_token_eos");
      return getEos();
    } catch (const DynamicLibraryException& e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException& e) {
      jni::throwJNIException(env, e);
    }
    return FAILURE;
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenNl(JNIEnv* env, jobject thisObject) {
    try {
      auto getNl = (get_special_token_pointer) getFunctionAddress("llama_token_nl");
      return getNl();
    } catch (const DynamicLibraryException& e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException& e) {
      jni::throwJNIException(env, e);
    }
    return FAILURE;
  }

}
