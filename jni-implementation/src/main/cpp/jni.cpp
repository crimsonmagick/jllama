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
    return static_cast<int32_t>(env->GetIntField(jInstance, fieldId));
  }

  uint32_t getUnsignedInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "I");
    if (!fieldId) {
      return 0;
    }
    return static_cast<uint32_t>(env->GetIntField(jInstance, fieldId));
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

  void throwNativeException(JNIEnv *env, const char* className,
                            const char* message) {
    jclass jExceptionClass = env->FindClass(className);
    if (jExceptionClass == nullptr) {
      std::cerr << "ERROR - Unable to lookup Java Exception class " << className
                << std::endl;
      return;
    }
    jmethodID constructor =
        env->GetMethodID(jExceptionClass, "<init>", "(Ljava/lang/String;)V");
    if (constructor == nullptr) {
      std::cerr << "ERROR - Unable to lookup constructor for class "
                << className << std::endl;
      return;
    }
    jstring jmsg = env->NewStringUTF(message);
    auto jException =
        (jthrowable) env->NewObject(jExceptionClass, constructor, jmsg);
    env->Throw(jException);
  }

  void throwDLLException(JNIEnv * env, const DynamicLibraryException& e) {
    throwNativeException(env,
                         "com/mangomelancholy/llama/cpp/java/bindings/exceptions/DynamicLibraryException", e.what());
  }

  void throwJNIException(JNIEnv *env, const JNIException &e) {
    throwNativeException(env,
                         "com/mangomelancholy/llama/cpp/java/bindings/exceptions/JNIException", e.what());
  }

  jobject constructLlamaOpaqueModel(JNIEnv *env, llama_model *modelPointer) {
    auto jmodelPointer = reinterpret_cast<jlong>(modelPointer);

    jclass llamaOpaqueModelClass = env->FindClass("com/mangomelancholy/llama/cpp/java/bindings/LlamaOpaqueModel");
    if (llamaOpaqueModelClass == nullptr) {
      throw JNIException("Unable to find LlamaOpaqueModel class");
    }

    jmethodID constructor = env->GetMethodID(llamaOpaqueModelClass, "<init>", "(J)V");
    if (constructor == nullptr) {
      throw JNIException("Unable to find LlamaOpaqueModel constructor");
    }

    jobject llamaOpaqueModelObj = env->NewObject(llamaOpaqueModelClass, constructor, jmodelPointer);
    if (llamaOpaqueModelObj == nullptr) {
      throw JNIException("Unable to initialize LlamaOpaqueModel");
    }
    return llamaOpaqueModelObj;
  }

  jobject constructLlamaOpaqueContext(JNIEnv* env, llama_context* jcontextPointer) {
    auto jContextPointer = reinterpret_cast<jlong>(jcontextPointer);

    jclass llamaOpaqueContextClass = env->FindClass("com/mangomelancholy/llama/cpp/java/bindings/LlamaOpaqueContext");
    if (llamaOpaqueContextClass == nullptr) {
      throw JNIException("Unable to find LlamaOpaqueContext class");
    }

    jmethodID constructor = env->GetMethodID(llamaOpaqueContextClass, "<init>", "(J)V");
    if (constructor == nullptr) {
      throw JNIException("Unable to find LlamaOpaqueContext constructor");
    }

    jobject llamaOpaqueContextObj = env->NewObject(llamaOpaqueContextClass, constructor, jcontextPointer);
    if (llamaOpaqueContextObj == nullptr) {
      throw JNIException("Unable to initialize LlamaOpaqueContext");
    }
    return llamaOpaqueContextObj;
  }

  llama_model* getLlamaModelPointer(JNIEnv* env, jobject llamaOpaqueModel) {
    jclass llamaOpaqueModelClass = env->FindClass("com/mangomelancholy/llama/cpp/java/bindings/LlamaOpaqueModel");
    if (llamaOpaqueModelClass == nullptr) {
      throw JNIException("Unable to find LlamaOpaqueModel class");
    }
    jfieldID fieldId = env->GetFieldID(llamaOpaqueModelClass, "modelPointer", "J");
    if (!fieldId) {
      throw JNIException("Unable to find modelPointer field for LlamaOpaqueModel class");
    }
    return reinterpret_cast<llama_model*>(env->GetLongField(llamaOpaqueModel, fieldId));
  }

}
