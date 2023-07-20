#include <stdio.h>
#include "com_mangomelancholy_llama_cpp_java_bindings_Main.h"

typedef void (*llama_backend_init)(bool);

#ifdef _WIN32
#include <windows.h>

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_initializeLlama
  (JNIEnv * env, jobject thisObject, jboolean useNuma) {
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

#elif defined(__unix__) || defined(__APPLE__)

#ifdef __APPLE__
#define LIBRARY_NAME "libllama.dylib"
#else
#define LIBRARY_NAME "libllama.so"
#endif

#include <dlfcn.h>

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_initializeLlama
  (JNIEnv * env, jobject thisObject, jboolean useNuma) {

    void* handle = dlopen(LIBRARY_NAME, RTLD_LAZY);

    if (!handle) {
        printf("could not load the dynamic library, error: %s\n", dlerror());
        return;
    }

    // clear any existing error
    dlerror();

    // resolve function address here
    llama_backend_init func = (llama_backend_init) dlsym(handle, "llama_backend_init");

    char *error = dlerror();
    if (error != NULL)  {
        printf("could not locate the function, error: %s\n", error);
        dlclose(handle);
        return;
    }

    printf("loaded function successfully!");

    try {
      func(useNuma);
      printf("Initialized llama.cpp backend\n");
    } catch (...) {
      printf("WARNING - Failed to initialized llama.cpp backend");
    }

    dlclose(handle);
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

