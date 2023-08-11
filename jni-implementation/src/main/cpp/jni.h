#ifndef LLAMA_CPP_JAVA_BINDINGS_JNI_H
#define LLAMA_CPP_JAVA_BINDINGS_JNI_H
#include "exceptions/dynamic-library-exception.h"

namespace jni {
  int32_t getInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  uint32_t getUnsignedInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  float getFloat(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  bool getBool(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  jfloatArray getJFloatArray(JNIEnv *env, jclass jType, jobject jInstance, const char* fieldName);
  void throwException(JNIEnv* env, const DynamicLibraryException& e);
}
#endif //LLAMA_CPP_JAVA_BINDINGS_JNI_H
