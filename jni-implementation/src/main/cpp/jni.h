#ifndef LLAMA_CPP_JAVA_BINDINGS_JNI_H
#define LLAMA_CPP_JAVA_BINDINGS_JNI_H
#include "exceptions/dynamic-library-exception.h"

struct llama_model;

namespace jni {

  class JNIException: public std::runtime_error {
    public:
    explicit JNIException(const char* message) : runtime_error(message) {

    }

  };

  int32_t getInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  uint32_t getUnsignedInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  float getFloat(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  bool getBool(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  jfloatArray getJFloatArray(JNIEnv *env, jclass jType, jobject jInstance, const char* fieldName);
  jobject constructLlamaOpaqueModel(JNIEnv* env, llama_model* modelPointer);
  void throwDLLException(JNIEnv* env, const DynamicLibraryException& e);
  void throwJNIException(JNIEnv* env, const JNIException& e);
}
#endif //LLAMA_CPP_JAVA_BINDINGS_JNI_H
