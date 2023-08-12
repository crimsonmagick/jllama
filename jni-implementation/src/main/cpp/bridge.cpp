#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl.h"
#include "exceptions/dynamic-library-exception.h"
#include "jni.h"
#include "libloader.h"
#include "llama.h"
#include "LlamaContextParamsManager.h"

typedef void (*llama_backend_init_pointer)(bool);
typedef void (*llama_backend_free_pointer)();
typedef llama_model *(*llama_load_model_from_file_pointer)
    (const char *, struct llama_context_params);

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

}
