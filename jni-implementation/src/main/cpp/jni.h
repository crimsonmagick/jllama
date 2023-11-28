#ifndef LLAMA_CPP_JAVA_BINDINGS_JNI_H
#define LLAMA_CPP_JAVA_BINDINGS_JNI_H
#include <jni.h>
#include "exceptions/DynamicLibraryException.h"
#include "exceptions/LlamaCppException.h"
#include "llama.h"

struct llama_model;
struct llama_context;

namespace jni {

  class JNIException: public std::runtime_error {
    public:
    explicit JNIException(const char* message) : runtime_error(message) {

    }

  };

  int32_t getInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  void setSignedInt32(int32_t value, JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  uint32_t getUnsignedInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  void setUnsignedInt32(uint32_t value, JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  float getFloat(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  void setFloat(float value, JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  bool getBool(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  void setBoolean(bool value, JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName);
  jfloatArray getJFloatArray(JNIEnv *env, jclass jType, jobject jInstance, const char* fieldName);
  jobject constructLlamaModel(JNIEnv* env, llama_model* modelPointer);
  jobject constructBatch(JNIEnv* env, jobject jContext, jint maxTokenCount, llama_batch* batch);
  llama_model* getLlamaModelPointer(JNIEnv* env, jobject llamaModel);
  void throwDLLException(JNIEnv* env, const DynamicLibraryException& e);
  void throwJNIException(JNIEnv* env, const JNIException& e);
  void throwLlamaCppException(JNIEnv* env, const LlamaCppException& e);
  void throwRuntimeException(JNIEnv* env, const std::exception& e);
  jobject constructLlamaContext(JNIEnv* env, llama_context* jcontextPointer);
  llama_context* getLlamaContextPointer(JNIEnv* env, jobject jLlamaContext);
  llama_batch* getLlamaBatchPointer(JNIEnv* env, jobject jBatch);
  llama_token_data_array getTokenDataArray(JNIEnv* env, jobject jTokenDataArray);
  void updateTokenDateArray(JNIEnv* env, jobject destination, llama_token_data_array* src);
}
#endif //LLAMA_CPP_JAVA_BINDINGS_JNI_H
