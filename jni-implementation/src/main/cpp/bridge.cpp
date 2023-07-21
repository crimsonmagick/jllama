#include <iostream>
#include "com_mangomelancholy_llama_cpp_java_bindings_Main.h"
#include "exceptions/dynamic-library-exception.h"
#include "libloader.h"

typedef void (*llama_backend_init)(bool);

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_initializeLlama
  (JNIEnv * env, jobject thisObject, jboolean useNuma) {
    try {
      loadLibrary("llamas");
      llama_backend_init func = (llama_backend_init) getFunctionAddress("llama_backend_init");
      func(useNuma);
      std::cout << "Initialized llama.cpp backend" << std::endl;
    } catch (const DynamicLibraryException& e) {
      // TODO rethrow exception as Java exception
    }
}


JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sayHello (JNIEnv * env, jobject thisObject) {
  std::cout << "Hello, World!" << std::endl;
}

JNIEXPORT jlong JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sum(JNIEnv * env, jobject thisObject, jint first, jint second) {
  return (long) first + (long) second;
}
