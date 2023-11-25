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

  jobject constructLlamaModel(JNIEnv *env, llama_model *modelPointer) {
    auto jmodelPointer = reinterpret_cast<jlong>(modelPointer);

    jclass llamaModelClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaModel");
    if (llamaModelClass == nullptr) {
      throw JNIException("Unable to find LlamaModel class");
    }

    jmethodID constructor = env->GetMethodID(llamaModelClass, "<init>", "(J)V");
    if (constructor == nullptr) {
      throw JNIException("Unable to find LlamaModel constructor");
    }

    jobject llamaModelObj = env->NewObject(llamaModelClass, constructor, jmodelPointer);
    if (llamaModelObj == nullptr) {
      throw JNIException("Unable to initialize LlamaModel");
    }
    return llamaModelObj;
  }

//  jobject constructBatch(JNIEnv* env, llama_batch batch) {
//    jclass jBatchClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaBatch");
//    if (jBatchClass == nullptr) {
//      throw JNIException("Unable to find LlamaBatch class");
//    }
//
//    jmethodID jConstructor = env->GetMethodID(jBatchClass, "<init>", "(J)V");
//    if (jConstructor == nullptr) {
//      throw JNIException("Unable to find LlamaBatch constructor");
//    }
//    jobject jBatch = env->NewObject(jBatchClass, jConstructor);
//    env->NewIntArray()
//
//  }

  jobject constructLlamaContext(JNIEnv* env, llama_context* jcontextPointer) {

    jclass llamaContextClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaContext");
    if (llamaContextClass == nullptr) {
      throw JNIException("Unable to find LlamaContext class");
    }

    jmethodID constructor = env->GetMethodID(llamaContextClass, "<init>", "(J)V");
    if (constructor == nullptr) {
      throw JNIException("Unable to find LlamaContext constructor");
    }

    jobject llamaContextObj = env->NewObject(llamaContextClass, constructor, jcontextPointer);
    if (llamaContextObj == nullptr) {
      throw JNIException("Unable to initialize LlamaContext");
    }
    return llamaContextObj;
  }

  llama_model* getLlamaModelPointer(JNIEnv* env, jobject llamaModel) {
    jclass llamaModelClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaModel");
    if (llamaModelClass == nullptr) {
      throw JNIException("Unable to find LlamaModel class");
    }
    jfieldID fieldId = env->GetFieldID(llamaModelClass, "modelPointer", "J");
    if (!fieldId) {
      throw JNIException("Unable to find modelPointer field for LlamaModel class");
    }
    return reinterpret_cast<llama_model*>(env->GetLongField(llamaModel, fieldId));
  }

  llama_context* getLlamaContextPointer(JNIEnv* env, jobject jLlamaContext) {
    jclass llamaContextClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaContext");
    if (llamaContextClass == nullptr) {
      throw JNIException("Unable to find LlamaContext class");
    }
    jfieldID fieldId = env->GetFieldID(llamaContextClass, "contextPointer", "J");
    if (!fieldId) {
      throw JNIException("Unable to find contextPointer field for LlamaContext class");
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
    size_t size = env->GetArrayLength(jDataArray);
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

    jni::setBoolean(src->sorted, env, jTokenDataArrayClass, destination, "sorted");

    jobjectArray jNewDataArray = env->NewObjectArray(src->size, jTokenDataClass, nullptr);

    if (!jNewDataArray) {
      throw JNIException("Unable to create new LlamaTokenData array");
    }

    for (int i = 0; i < src->size; i++) {
      jobject jTokenData = env->AllocObject(jTokenDataClass);
      if (!jTokenData) {
          throw JNIException("Unable to allocate new LlamaTokenData");
      }
      env->SetIntField(jTokenData, tokenFieldId, src->data[i].id);
      env->SetFloatField(jTokenData, logitFieldId, src->data[i].logit);
      env->SetFloatField(jTokenData, pFieldId, src->data[i].p);
      env->SetObjectArrayElement(jNewDataArray, i, jTokenData);
      env->DeleteLocalRef(jTokenData);
    }
    env->SetObjectField(destination, jArrayFieldId, jNewDataArray);
  }

}
