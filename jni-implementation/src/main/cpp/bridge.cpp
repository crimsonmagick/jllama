#include <iostream>
#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl.h"
#include "exceptions/dynamic-library-exception.h"
#include "jni.h"
#include "libloader.h"
#include "../../../../external/llama.cpp/llama.h"

typedef void (*llama_backend_init_pointer)(bool);
typedef void (*llama_backend_free_pointer)();
typedef llama_model* (*llama_load_model_from_file_pointer)(const char*, struct llama_context_params);

extern "C" {

  JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_loadLibrary(JNIEnv* env, jobject thisObject) {

    try {
      loadLibrary("llama");
    } catch (const DynamicLibraryException& e) {
      jni::throwException(env, e);
    }
  }

  JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_closeLibrary(JNIEnv* env, jobject thisObject) {
    try {
      closeLibrary();
    } catch (const DynamicLibraryException& e) {
      jni::throwException(env, e);
    }
  }

  JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaBackendInit(JNIEnv* env, jobject thisObject, jboolean useNuma) {
    try {
      llama_backend_init_pointer func = (llama_backend_init_pointer) getFunctionAddress("llama_backend_init");
      func(useNuma);
      std::cout << "Initialized llama.cpp backend\n";
    } catch (const DynamicLibraryException& e) {
      jni::throwException(env, e);
    }
  }

  JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaBackendFree(JNIEnv* env, jobject thisObject) {
    try {
      llama_backend_free_pointer llamaFree = (llama_backend_free_pointer) getFunctionAddress("llama_backend_free");
      llamaFree();
      std::cout << "llama freed\n";
    } catch (const DynamicLibraryException& e) {
      jni::throwException(env, e);
    }
  }
}


//JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sayHello (JNIEnv * env, jobject thisObject) {
//  std::cout << "Hello, World!" << std::endl;
//}
//
//JNIEXPORT jlong JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sum(JNIEnv * env, jobject thisObject, jint first, jint second) {
//  return (long) first + (long) second;
//}
