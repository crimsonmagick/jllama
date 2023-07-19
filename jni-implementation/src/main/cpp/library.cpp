#include <stdio.h>
#include "com_mangomelancholy_llama_cpp_java_bindings_Main.h"

#ifdef _WIN32
#include <windows.h>

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_initializeLlama
  (JNIEnv * env, jobject thisObject, jboolean useNuma) {
    typedef void (*llama_backend_init)(bool);
    HINSTANCE hLlamaDLL = NULL;
    hLlamaDLL = LoadLibrary("llama.dll");

    if (!hLlamaDLL) {
      printf("could not load the dynamic library, error: %d\n", GetLastError());
      return;
    }

    // resolve function address here
    llama_backend_init func = (llama_backend_init) GetProcAddress(hLlamaDLL, "llama_backend_init");
    if (!func) {
      printf("could not locate the function\n");
    } else {
      printf("loaded function successfully!\n");
    }
    try {
      func(useNuma);
      printf("Initialized llama.cpp backend\n");
    } catch (...) {
      printf("WARNING - Failed to initialized llama.cpp backend");
    }
    FreeLibrary(hLlamaDLL);
}

#elif defined(__unix__)

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_initializeLlama
  (JNIEnv * env, jobject thisObject, jboolean useNuma) {
    printf("WARNING - UNIX-LIKE ARCHITECTURE NOT SUPPORTED\n");
}

#else
JNIEXPORT void JNICALL
Java_com_mangomelancholy_llama_cpp_java_bindings_Main_initializeLlama
(JNIEnv
* env,
jobject thisObject, jboolean
useNuma) {
printf("WARNING - ARCHITECTURE NOT SUPPORTED\n");
}
#endif

JNIEXPORT void JNICALL
Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sayHello (JNIEnv
* env,
jobject thisObject
) {
printf("Hello, World!\n");
}

JNIEXPORT jlong
JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sum
    (JNIEnv * env, jobject
thisObject,
jint first, jint
second) {
return (long) first + (long)
second;
}

