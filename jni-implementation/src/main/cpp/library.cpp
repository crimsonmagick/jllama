#include "com_mangomelancholy_llama_cpp_java_bindings_Main.h"

#include <stdio.h>

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sayHello (JNIEnv* env, jobject thisObject) {
    printf("Hello, World!\n");
 }