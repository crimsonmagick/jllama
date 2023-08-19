#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl.h"
#include "exceptions/dynamic-library-exception.h"
#include "jni.h"
#include "libloader.h"
#include "llama.h"
#include "LlamaContextParamsManager.h"
#include "Utf8StringManager.h"

const int FAILURE = 1;

typedef void (* llama_backend_init_pointer)(bool);
typedef void (* llama_backend_free_pointer)();
typedef llama_model* (* llama_load_model_from_file_pointer)
    (const char*, struct llama_context_params);
typedef llama_context* (* llama_new_context_with_model_pointer)
    (llama_model*, llama_context_params);
typedef int(* llama_tokenize_with_model_pointer)
    (llama_model*, const char*, llama_token*, int, bool);
typedef int(* llama_eval_pointer)(llama_context*, llama_token*, int, int, int);
typedef const char* (* llama_token_to_str_pointer)(llama_context*, llama_token);
typedef float* (* llama_get_logits_pointer)(llama_context*);
typedef int (* llama_n_vocab_pointer)(const struct llama_context*);
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
    try {
      llama_backend_init_pointer func =
          (llama_backend_init_pointer) getFunctionAddress("llama_backend_init");
      func(useNuma);
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaBackendFree(
      JNIEnv *env,
      jobject thisObject) {
    try {
      llama_backend_free_pointer llamaFree =
          (llama_backend_free_pointer) getFunctionAddress("llama_backend_free");
      llamaFree();
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT jobject
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLoadModelFromFile(
      JNIEnv *env,
      jobject thisObject,
      jbyteArray path,
      jobject javaParams) {

    try {
      llama_load_model_from_file_pointer llamaLoadModel =
          (llama_load_model_from_file_pointer) getFunctionAddress(
              "llama_load_model_from_file");

      auto paramsManager = LlamaContextParamsManager(env, path, javaParams);

      llama_model* model = llamaLoadModel(paramsManager.getPath(), paramsManager.getParams());

      if (model) {
        return jni::constructLlamaOpaqueModel(env, model);
      }
      jni::throwJNIException(env, jni::JNIException("Unable to load llama model from file"));
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException &e) {
      jni::throwJNIException(env, e);
    }
    return nullptr;
  }

  JNIEXPORT jobject
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaLoadContextWithModel
      (JNIEnv* env, jobject thisObject, jobject jllamaModel, jobject jContextParams) {

    try {
      llama_new_context_with_model_pointer llamaCreateContext =
          (llama_new_context_with_model_pointer) getFunctionAddress(
              "llama_new_context_with_model");

      auto paramsManager = LlamaContextParamsManager(env, jContextParams);
      auto llamaModel = jni::getLlamaModelPointer(env, jllamaModel);
      llama_context* context = llamaCreateContext(llamaModel, paramsManager.getParams());

      if (context) {
        return jni::constructLlamaOpaqueContext(env, context);
      }
      jni::throwJNIException(env, jni::JNIException("Unable to create context from llama model"));
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException &e) {
      jni::throwJNIException(env, e);
    }
    return nullptr;
  }

  JNIEXPORT jint
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaTokenizeWithModel
      (JNIEnv* env,
       jobject thisObject,
       jobject jllamaModel,
       jbyteArray jToTokenize,
       jintArray jTokensOut,
       jint jmaxTokens,
       jboolean jBos) {
    try {
      llama_tokenize_with_model_pointer tokenize =
          (llama_tokenize_with_model_pointer) getFunctionAddress(
              "llama_tokenize_with_model");

      auto llamaModel = jni::getLlamaModelPointer(env, jllamaModel);

      auto stringManager = Utf8StringManager(env, jToTokenize);
      auto toTokenize = stringManager.getUtf8String();
      jint* tokensOut = env->GetIntArrayElements(jTokensOut, nullptr);
      int result = tokenize(llamaModel,
                            toTokenize,
                            reinterpret_cast<llama_token*>(tokensOut),
                            jmaxTokens,
                            jBos);
      if (result > 0 ) {
        env->ReleaseIntArrayElements(jTokensOut, tokensOut, 0);
      } else {
        env->ReleaseIntArrayElements(jTokensOut, tokensOut, JNI_ABORT);
      }
      return result;
    } catch (const DynamicLibraryException& e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException& e) {
      jni::throwJNIException(env, e);
    }
    return FAILURE;
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaEval(
      JNIEnv* env,
      jobject thisObject,
      jobject jContext,
      jintArray jTokens,
      jint jnTokens,
      jint jnPast,
      jint jnThreads) {
    try {
      llama_eval_pointer eval = (llama_eval_pointer) getFunctionAddress(
          "llama_eval");
      auto llamaContext = jni::getLlamaContextPointer(env, jContext);
      jint* tokens = env->GetIntArrayElements(jTokens, nullptr);
      int result = eval(llamaContext, reinterpret_cast<int*>(tokens), jnTokens, jnPast, jnThreads);
      env->ReleaseIntArrayElements(jTokens, tokens, JNI_ABORT);
      return result;
    } catch (const DynamicLibraryException& e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException& e) {
      jni::throwJNIException(env, e);
    }
    return FAILURE;
  }

  JNIEXPORT jfloatArray JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaCppJNIImpl_llamaGetLogits(JNIEnv * env, jobject thisObject, jobject jContext) {
    try {
      auto getLogits = (llama_get_logits_pointer) getFunctionAddress(
          "llama_get_logits");
      auto getVocabLength = (llama_n_vocab_pointer) getFunctionAddress("llama_n_vocab");
      auto llamaContext = jni::getLlamaContextPointer(env, jContext);
      float* logits = getLogits(llamaContext);
      int vocabLength = getVocabLength(llamaContext);
      auto jLogits = env->NewFloatArray(vocabLength);
      env->SetFloatArrayRegion(jLogits, 0, vocabLength, logits);
      return jLogits;
    } catch (const DynamicLibraryException& e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException& e) {
      jni::throwJNIException(env, e);
    }
    return nullptr;
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
