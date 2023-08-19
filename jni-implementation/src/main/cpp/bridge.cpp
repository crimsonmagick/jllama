#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl.h"
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

extern "C" {

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_loadLibrary(
      JNIEnv *env,
      jobject thisObject) {

    try {
      loadLibrary("llama");
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_closeLibrary(
      JNIEnv *env,
      jobject thisObject) {
    try {
      closeLibrary();
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaBackendInit(
      JNIEnv *env,
      jobject thisObject,
      jboolean useNuma) {
    // TODO move to build?
    if (sizeof(jint) != sizeof(llama_token)) {
      jni::throwJNIException(env, jni::JNIException("Java int value is the not the same size as a token. Aborting init..."));
      return;
    }
    try {
      llama_backend_init_pointer func =
          (llama_backend_init_pointer) getFunctionAddress("llama_backend_init");
      func(useNuma);
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaBackendFree(
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
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaLoadModelFromFile(
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
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaLoadContextWithModel
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
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaTokenizeWithModel
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

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaEval(
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

  JNIEXPORT jfloatArray JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaGetLogits(JNIEnv * env, jobject thisObject, jobject jContext) {
    return env->NewFloatArray(0);
  }

  JNIEXPORT jint JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaSampleTokenGreedy (JNIEnv* env, jobject thisObject, jobject, jobject jCandidates) {
    return 0;
  }

  JNIEXPORT jbyteArray JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaTokenToStr(JNIEnv* env, jobject thisObject, jobject jContext, jint jToken) {
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
//    return env->NewByteArray(0);
  }

}
