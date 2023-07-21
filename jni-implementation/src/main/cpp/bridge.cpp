#include <stdio.h>
#include "com_mangomelancholy_llama_cpp_java_bindings_Main.h"
#include "libloader.h"

typedef void (*llama_backend_init)(bool);

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_initializeLlama
  (JNIEnv * env, jobject thisObject, jboolean useNuma) {
    loadLibrary("llama");

    llama_backend_init func = (llama_backend_init) getFunctionAddress("llama_backend_init");
    if (!func) {
      printf("could not load the function\n");
      return;
    } else {
      printf("loaded function successfully!\n");
    }
    try {
      func(useNuma);
      printf("Initialized llama.cpp backend\n");
    } catch (...) {
      printf("WARNING - Failed to initialized llama.cpp backend");
    }
}


JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sayHello (JNIEnv * env, jobject thisObject) {
  printf("Hello, World!\n");
}

JNIEXPORT jlong JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sum(JNIEnv * env, jobject thisObject, jint first, jint second) {
  return (long) first + (long) second;
}
