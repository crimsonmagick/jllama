#include <cstdio>
#include <iostream>
#include <jni.h>
#include "jni.h"
#include "exceptions/DynamicLibraryException.h"

std::string fieldNotFound(const char* fieldName) {
  return "Unable to set field with fieldName=" + std::string(fieldName);
}

namespace jni {

  int32_t getInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "I");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    return static_cast<int32_t>(env->GetIntField(jInstance, fieldId));
  }

  void setSignedInt32(int32_t value,
                        JNIEnv* env,
                        jclass jType,
                        jobject jInstance,
                        const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "I");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    env->SetIntField(jInstance, fieldId, value);
  }

  uint32_t getUnsignedInt32(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "I");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    return static_cast<uint32_t>(env->GetIntField(jInstance, fieldId));
  }

  void setUnsignedInt32(uint32_t value,
                        JNIEnv* env,
                        jclass jType,
                        jobject jInstance,
                        const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "I");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    env->SetIntField(jInstance, fieldId, value);
  }

  size_t getSizeT(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "J");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    return static_cast<size_t>(env->GetIntField(jInstance, fieldId));
  }

  float getFloat(JNIEnv *env, jclass jType, jobject jInstance, const char *fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "F");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    return static_cast<float>(env->GetFloatField(jInstance, fieldId));
  }

  void setFloat(float value,
                 JNIEnv* env,
                 jclass jType,
                 jobject jInstance,
                 const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "F");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    env->SetFloatField(jInstance, fieldId, value);
  }

  bool getBool(JNIEnv *env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "Z");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    return static_cast<bool>(env->GetBooleanField(jInstance, fieldId));
  }

  void setBoolean(bool value,
                JNIEnv* env,
                jclass jType,
                jobject jInstance,
                const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "Z");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    env->SetBooleanField(jInstance, fieldId, value);
  }

  // WARNING must release const float * with ReleaseFloatArrayElements()
  jfloatArray getJFloatArray(JNIEnv *env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "[F");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    return (jfloatArray) env->GetObjectField(jInstance, fieldId);
  }

  void throwNativeException(JNIEnv *env, const char* className,
                            const char* message) {
    jclass jExceptionClass = env->FindClass(className);
    if (jExceptionClass == nullptr) {
      std::string errorMessage = "ERROR - Unable to lookup Java Exception class " + std::string(className);
      std::cerr << errorMessage << std::endl;
      throw JNIException(errorMessage.c_str());
    }
    jmethodID constructor =
        env->GetMethodID(jExceptionClass, "<init>", "(Ljava/lang/String;)V");
    if (constructor == nullptr) {
      std::string errorMessage = "ERROR - Unable to lookup constructor for class " + std::string(className);
      std::cerr << errorMessage << std::endl;
      throw JNIException(errorMessage.c_str());
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
  void updateTokenDateArray(JNIEnv* env,
                            jobject destination,
                            llama_token_data_array* src) {
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

    auto jDataArray = reinterpret_cast<jobjectArray>(env->GetObjectField(destination, jArrayFieldId));
    jsize jSize = env->GetArrayLength(jDataArray);
    if (jSize != src->size) {
      throw JNIException("TokenDataArray data.length must match llama_token_data_array.size");
    }
    jni::setBoolean(src->sorted, env, jTokenDataArrayClass, destination, "sorted");

    for (int i = 0; i < src->size; i++) {
      jobject jTokenData = env->GetObjectArrayElement(jDataArray, i);
      env->SetFloatField(jTokenData, logitFieldId, src->data[i].logit);
      env->SetFloatField(jTokenData, pFieldId, src->data[i].p);

      env->DeleteLocalRef(jTokenData);
    }
  }

}
