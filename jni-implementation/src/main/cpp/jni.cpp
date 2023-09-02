#include <iostream>
#include <jni.h>
#include "jni.h"
#include "exceptions/DynamicLibraryException.h"

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

  size_t getSizeT(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "J");
    if (!fieldId) {
      return 0;
    }
    return static_cast<size_t>(env->GetIntField(jInstance, fieldId));
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
                         "net/jllama/llama/cpp/java/bindings/exceptions/LlamaCppException", e.what());
  }

  void throwJNIException(JNIEnv *env, const JNIException &e) {
    throwNativeException(env, "net/jllama/llama/cpp/java/bindings/exceptions/JNIException", e.what());
  }

  void throwLlamaCppException(JNIEnv* env, const LlamaCppException& e) {
    throwNativeException(env, "net/jllama/llama/cpp/java/bindings/exceptions/LlamaCppException", e.what());
  }

  void throwRuntimeException(JNIEnv* env, const std::exception& e) {
    throwNativeException(env, "java/lang/RuntimeException", e.what());
  }

  jobject constructLlamaOpaqueModel(JNIEnv *env, llama_model *modelPointer) {
    auto jmodelPointer = reinterpret_cast<jlong>(modelPointer);

    jclass llamaOpaqueModelClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaOpaqueModel");
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

    jclass llamaOpaqueContextClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaOpaqueContext");
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
    jclass llamaOpaqueModelClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaOpaqueModel");
    if (llamaOpaqueModelClass == nullptr) {
      throw JNIException("Unable to find LlamaOpaqueModel class");
    }
    jfieldID fieldId = env->GetFieldID(llamaOpaqueModelClass, "modelPointer", "J");
    if (!fieldId) {
      throw JNIException("Unable to find modelPointer field for LlamaOpaqueModel class");
    }
    return reinterpret_cast<llama_model*>(env->GetLongField(llamaOpaqueModel, fieldId));
  }
  llama_context* getLlamaContextPointer(JNIEnv* env, jobject jLlamaContext) {
    jclass llamaOpaqueContextClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaOpaqueContext");
    if (llamaOpaqueContextClass == nullptr) {
      throw JNIException("Unable to find LlamaOpaqueContext class");
    }
    jfieldID fieldId = env->GetFieldID(llamaOpaqueContextClass, "contextPointer", "J");
    if (!fieldId) {
      throw JNIException("Unable to find contextPointer field for LlamaOpaqueContext class");
    }
    return reinterpret_cast<llama_context*>(env->GetLongField(jLlamaContext, fieldId));
  }

  llama_token_data_array getTokenDataArray(JNIEnv* env, jobject jTokenDataArray) {
    jclass jTokenDataArrayClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaTokenDataArray");
    if (jTokenDataArrayClass == nullptr) {
      throw JNIException("Unable to find LlamaTokenDataArray class");
    }
    jfieldID jArrayFieldId = env->GetFieldID(jTokenDataArrayClass, "data", "[Lnet/jllama/llama/cpp/java/bindings/LlamaTokenData;");
    if (jArrayFieldId == nullptr) {
      throw JNIException("Unable to find LlamaTokenData array \"data\"");
    }

    jclass jTokenDataClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaTokenData");
    if (jTokenDataClass == nullptr) {
      throw JNIException("Unable to find jTokenDataClass class");
    }
    jfieldID tokenFieldId = env->GetFieldID(jTokenDataClass, "llamaToken", "I");
    if (tokenFieldId == nullptr) {
      throw JNIException("Unable to find LlamaTokenData field `\"llamaToken\"");
    }
    jfieldID logitFieldId = env->GetFieldID(jTokenDataClass, "logit", "F");
    if (logitFieldId == nullptr) {
      throw JNIException("Unable to find LlamaTokenData field `\"logit\"");
    }
    jfieldID pFieldId = env->GetFieldID(jTokenDataClass, "p", "F");
    if (pFieldId == nullptr) {
      throw JNIException("Unable to find LlamaTokenData field `\"p\"");
    }

    auto jDataArray = reinterpret_cast<jobjectArray>(env->GetObjectField(jTokenDataArray, jArrayFieldId));
    size_t size = jni::getSizeT(env, jTokenDataArrayClass, jTokenDataArray, "size");
    bool sorted = jni::getBool(env, jTokenDataArrayClass, jTokenDataArray, "sorted");

    auto dataArray = new llama_token_data[size];
    for (jint i = 0; i < size; i++) {
      jobject jTokenData = env->GetObjectArrayElement(jDataArray, i);
      llama_token token = env->GetIntField(jTokenData, tokenFieldId);
      float logit = env->GetFloatField(jTokenData, logitFieldId);
      float p = env->GetFloatField(jTokenData, pFieldId);
      dataArray[i] = llama_token_data {
          token, logit, p
      };
      env->DeleteLocalRef(jTokenData);
    }

    return llama_token_data_array {
      dataArray, size, sorted
    };
  }

}
