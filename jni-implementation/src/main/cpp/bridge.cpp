#include <iostream>
#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl.h"
#include "exceptions/dynamic-library-exception.h"
#include "jni.h"
#include "libloader.h"

typedef void (*llama_backend_init)(bool);
typedef void (*llama_backend_free)();

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_initializeLlama(JNIEnv* env, jobject thisObject, jboolean useNuma) {
  try {
    loadLibrary("llama");
    llama_backend_init func = (llama_backend_init) getFunctionAddress("llama_backend_init");
    func(useNuma);
    std::cout << "Initialized llama.cpp backend" << std::endl;
  } catch (const DynamicLibraryException& e) {
    jni::throwException(env, e);
  }
}

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_terminateLlama(JNIEnv* env, jobject thisObject) {
  try {
    llama_backend_free llamaFree = (llama_backend_free) getFunctionAddress("llama_backend_free");
    llamaFree();
    closeLibrary();
    std::cout << "llama freed, dll terminated" << std::endl;
  } catch (const DynamicLibraryException& e) {
    jni::throwException(env, e);
  }
}

//JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sayHello (JNIEnv * env, jobject thisObject) {
//  std::cout << "Hello, World!" << std::endl;
//}
//
//JNIEXPORT jlong JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sum(JNIEnv * env, jobject thisObject, jint first, jint second) {
//  return (long) first + (long) second;
//}
