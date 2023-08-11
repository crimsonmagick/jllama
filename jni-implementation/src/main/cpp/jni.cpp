#include <iostream>
#include <jni.h>
#include "jni.h"
#include "exceptions/dynamic-library-exception.h"

namespace jni {

  int32_t getInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "I");
    if (!fieldId) {
      return 0;
    }
    return static_cast<int32_t>(env->GetLongField(jInstance, fieldId));
  }

  uint32_t getUnsignedInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "I");
    if (!fieldId) {
      return 0;
    }
    return static_cast<uint32_t>(env->GetIntField(jInstance, fieldId));
  }

  void throwException(JNIEnv * env, const DynamicLibraryException& e) {
    jclass jExceptionClass = env->FindClass("com/mangomelancholy/llama/cpp/java/bindings/exceptions/DynamicLibraryException");
    if (jExceptionClass == nullptr) {
      std::cerr << "ERROR - Unable to lookup Java Exception class DynamicLibraryException" << std::endl;
      return;
    }
    jmethodID constructor = env->GetMethodID(jExceptionClass, "<init>", "(Ljava/lang/String;)V");
    if (constructor == nullptr) {
      std::cerr << "ERROR - Unable to lookup constructor for class DynamicLibraryException" << std::endl;
      return;
    }
    jstring jmsg = env->NewStringUTF(e.what());
    auto jException = (jthrowable) env->NewObject(jExceptionClass, constructor, jmsg);
    env->Throw(jException);
  }
  float getFloat(JNIEnv *env, jclass jType, jobject jInstance, const char *fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "F");
    if (!fieldId) {
      return 0;
    }
    return static_cast<float>(env->GetFloatField(jInstance, fieldId));
  }
  bool getBool(JNIEnv *env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "Z");
    if (!fieldId) {
      return false;
    }
    return static_cast<bool>(env->GetBooleanField(jInstance, fieldId));
  }

  // WARNING must release const float * with ReleaseFloatArrayElements()
  jfloatArray getJFloatArray(JNIEnv *env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "[F");
    return (jfloatArray) env->GetObjectField(jInstance, fieldId);
  }
}
