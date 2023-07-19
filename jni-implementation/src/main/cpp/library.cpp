#include "com_mangomelancholy_llama_cpp_java_bindings_Main.h"

#include <stdio.h>

JNIEXPORT void JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sayHello (JNIEnv* env, jobject thisObject) {
    printf("Hello, World!\n");
 }

JNIEXPORT jlong JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_Main_sum (JNIEnv* env, jobject thisObject, jint first, jint second) {
  return (long) first + (long) second;
}