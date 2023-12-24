#include <iostream>
#include <jni.h>
#include <memory>
#include <sstream>
#include "jni.h"
#include "exceptions/DynamicLibraryException.h"

std::string fieldNotFound(const char* fieldName) {
  return "Unable to set field with fieldName=" + std::string(fieldName);
}



namespace jni {

  enum ARRAY_TYPE {
    BYTE, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, BOOLEAN, OBJECT
  };

  struct ArrayHelper {
    std::string getJavaTypeName(ARRAY_TYPE type) {
      std::string typeName;
      switch(type) {
        case BYTE:
          typeName="byte";
          break;
        case INT:
          typeName = "int";
          break;
        case LONG:
          typeName = "long";
          break;
        case FLOAT:
          typeName = "float";
          break;
        case DOUBLE:
          typeName = "double";
          break;
        case BOOLEAN:
          typeName = "boolean";
          break;
        case OBJECT:
          typeName = "Object";
          break;
      }
      return typeName;
    }
  } ArrayHelper;

  void validateArray(JNIEnv* env, _jarray* jArray, jint size, ARRAY_TYPE type) {
    jthrowable jException = env->ExceptionOccurred();
    if (jException) {
      std::ostringstream errorMessage;
      errorMessage << "Failed to create new " << ArrayHelper.getJavaTypeName(type) << "[] array with size=" << size << ".";
      env->ExceptionClear();
      throw JNIException(errorMessage.str().c_str(), jException);
    }
    if (!jArray) {
      std::ostringstream errorMessage;
      errorMessage << "Failed to create new " << ArrayHelper.getJavaTypeName(type) << "[] array with size=" << size << ", reason unknown.";
      throw JNIException(errorMessage.str().c_str());
    }
  }

  template<>
  jbooleanArray newPrimitiveArray<jbooleanArray>(JNIEnv* env, jint size) {
    jbooleanArray newArray = env->NewBooleanArray(size);
    validateArray(env, newArray, size, ARRAY_TYPE::BOOLEAN);
    return newArray;
  }

  template<>
  jbyteArray newPrimitiveArray<jbyteArray>(JNIEnv* env, jint size) {
    jbyteArray newArray = env->NewByteArray(size);
    validateArray(env, newArray, size, ARRAY_TYPE::BYTE);
    return newArray;
  }

  template<>
  jcharArray newPrimitiveArray<jcharArray>(JNIEnv* env, jint size) {
    jcharArray newArray = env->NewCharArray(size);
    validateArray(env, newArray, size, ARRAY_TYPE::CHAR);
    return newArray;
  }

  template<>
  jshortArray newPrimitiveArray<jshortArray>(JNIEnv* env, jint size) {
    jshortArray newArray = env->NewShortArray(size);
    validateArray(env, newArray, size, ARRAY_TYPE::SHORT);
    return newArray;
  }

  template<>
  jintArray newPrimitiveArray<jintArray>(JNIEnv* env, jint size) {
    jintArray newArray = env->NewIntArray(size);
    validateArray(env, newArray, size, ARRAY_TYPE::INT);
    return newArray;
  }

  template<>
  jlongArray newPrimitiveArray<jlongArray>(JNIEnv* env, jint size) {
    jlongArray newArray = env->NewLongArray(size);
    validateArray(env, newArray, size, ARRAY_TYPE::LONG);
    return newArray;
  }

  template<>
  jfloatArray newPrimitiveArray<jfloatArray>(JNIEnv* env, jint size) {
    jfloatArray newArray = env->NewFloatArray(size);
    validateArray(env, newArray, size, ARRAY_TYPE::FLOAT);
    return newArray;
  }

template<>
jdoubleArray newPrimitiveArray<jdoubleArray>(JNIEnv* env, jint size) {
  jdoubleArray newArray = env->NewDoubleArray(size);
  validateArray(env, newArray, size, ARRAY_TYPE::DOUBLE);
  return newArray;
}

jobjectArray newObjectArray(JNIEnv* env, jint size, jclass memberType) {
  jobjectArray newArray = env->NewObjectArray(size, memberType, nullptr);
  validateArray(env, newArray, size, ARRAY_TYPE::OBJECT);
  return newArray;
}

  int8_t getByte(JNIEnv* env, jclass jType, jobject jInstance, const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "B");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    return static_cast<int8_t>(env->GetByteField(jInstance, fieldId));
  }

  void setByte(int8_t value,
               JNIEnv* env,
               jclass jType,
               jobject jInstance,
               const char* fieldName) {
    jfieldID fieldId = env->GetFieldID(jType, fieldName, "B");
    if (!fieldId) {
      throw JNIException(fieldNotFound(fieldName).c_str());
    }
    env->SetByteField(jInstance, fieldId, value);
  }

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
                         "net/jllama/core/exceptions/LlamaCppException", e.what());
  }

  void throwJNIException(JNIEnv *env, const JNIException &e) {
    throwNativeException(env, "net/jllama/core/exceptions/JNIException", e.what());
  }

  void throwLlamaCppException(JNIEnv* env, const LlamaCppException& e) {
    throwNativeException(env, "net/jllama/core/exceptions/LlamaCppException", e.what());
  }

  void throwRuntimeException(JNIEnv* env, const std::exception& e) {
    throwNativeException(env, "java/lang/RuntimeException", e.what());
  }

  jobject constructLlamaModel(JNIEnv *env, llama_model *modelPointer) {
    auto jmodelPointer = reinterpret_cast<jlong>(modelPointer);

    jclass llamaModelClass = env->FindClass("net/jllama/core/LlamaModel");
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

  jobject constructBatchOld(JNIEnv* env, jobject jContext, jint maxTokenCount, llama_batch* batch) {
    auto jBatchPointer = reinterpret_cast<jlong>(batch);
    jclass jBatchClass = env->FindClass("net/jllama/core/LlamaContext$LlamaBatchOld");
    if (jBatchClass == nullptr) {
      throw JNIException("Unable to find LlamaBatch class");
    }

    jmethodID jConstructor = env->GetMethodID(jBatchClass, "<init>", "(Lnet/jllama/core/LlamaContext;JI)V");
    if (jConstructor == nullptr) {
      throw JNIException("Unable to find LlamaBatch constructor");
    }
    jobject jBatch = env->NewObject(jBatchClass, jConstructor, jContext, jBatchPointer, maxTokenCount);
    if (jBatch == nullptr) {
      throw JNIException("Unable to initialize LlamaBatch");
    }
    return jBatch;
  }

  jobject constructBatch(JNIEnv* env, jobject jContext,  llama_batch *batch, jint jNTokens, jint jEmbd, jint nSeqId) {
    auto jBatchPointer = reinterpret_cast<jlong>(batch);
    jclass jBatchClass = env->FindClass("net/jllama/core/LlamaContext$LlamaBatch");
    if (jBatchClass == nullptr) {
      throw JNIException("Unable to find LlamaBatch class");
    }
    jfloatArray jEmbdArray;
    jintArray jTokenArray;
    jint jLength;
    if (jEmbd) {
      jLength = jEmbd;
      jEmbdArray = newPrimitiveArray<jfloatArray>(env, jLength);
      jTokenArray = nullptr;
    } else {
      jLength = jNTokens;
      jTokenArray = newPrimitiveArray<jintArray>(env, jLength);
      jEmbdArray = nullptr;
    }
    jintArray jNSeqIdArray = newPrimitiveArray<jintArray>(env, jLength);

    // initialize the 2d seqId array
    jobjectArray jSeqIdArray = newObjectArray(env, jLength, env->FindClass("[I"));
    std::unique_ptr<jint[]> fill(new jint[nSeqId]);
    for (int i = 0; i < nSeqId; i++) {
      fill[i] = 0;
    }
    for (int i = 0; i < jLength; i++) {
      jintArray jSingleSeqArray = newPrimitiveArray<jintArray>(env, nSeqId);
      env->SetIntArrayRegion(jSingleSeqArray, 0, nSeqId, fill.get());
      env->SetObjectArrayElement(jSeqIdArray, i, jSingleSeqArray);
      env->DeleteLocalRef(jSingleSeqArray);
    }

    jintArray jPos = newPrimitiveArray<jintArray>(env, jLength);
    jbyteArray jLogits = newPrimitiveArray<jbyteArray>(env, jLength);

    jmethodID jConstructor = env->GetMethodID(jBatchClass, "<init>", "(Lnet/jllama/core/LlamaContext;JI[I[F[I[I[[I[B)V");
    if (jConstructor == nullptr) {
      throw JNIException("Unable to find LlamaBatch constructor");
    }
    jobject jBatch = env->NewObject(jBatchClass, jConstructor, jContext,
                                    jBatchPointer, batch->n_tokens, jTokenArray,
                                    jEmbdArray, jPos, jNSeqIdArray, jSeqIdArray,
                                    jLogits);
    if (jBatch == nullptr) {
      throw JNIException("Unable to initialize LlamaBatch");
    }
    return jBatch;
  }

  jobject constructLlamaContext(JNIEnv* env, llama_context* jcontextPointer) {

    jclass llamaContextClass = env->FindClass("net/jllama/core/LlamaContext");
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
    jclass llamaModelClass = env->FindClass("net/jllama/core/LlamaModel");
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
    jclass llamaContextClass = env->FindClass("net/jllama/core/LlamaContext");
    if (llamaContextClass == nullptr) {
      throw JNIException("Unable to find LlamaContext class");
    }
    jfieldID fieldId = env->GetFieldID(llamaContextClass, "contextPointer", "J");
    if (!fieldId) {
      throw JNIException("Unable to find contextPointer field for LlamaContext class");
    }
    return reinterpret_cast<llama_context*>(env->GetLongField(jLlamaContext, fieldId));
  }

  llama_batch* getLlamaBatchPointer(JNIEnv* env, jobject jBatch) {
    jclass llamaBatchClass = env->FindClass("net/jllama/core/LlamaContext$LlamaBatch");
    if (llamaBatchClass == nullptr) {
      throw JNIException("Unable to find LlamaContext class");
    }
    jfieldID fieldId = env->GetFieldID(llamaBatchClass, "batchPointer", "J");
    if (!fieldId) {
      throw JNIException("Unable to find batchPointer field for LlamaBatch class");
    }
    return reinterpret_cast<llama_batch*>(env->GetLongField(jBatch, fieldId));
  }

  llama_token_data_array getTokenDataArray(JNIEnv* env, jobject jTokenDataArray) {
    jclass jTokenDataArrayClass = env->FindClass("net/jllama/core/LlamaTokenDataArray");
    if (jTokenDataArrayClass == nullptr) {
      throw JNIException("Unable to find LlamaTokenDataArray class");
    }
    jfieldID jArrayFieldId = env->GetFieldID(jTokenDataArrayClass, "data", "[Lnet/jllama/core/LlamaTokenData;");
    if (jArrayFieldId == nullptr) {
      throw JNIException("Unable to find LlamaTokenData array \"data\"");
    }

    jclass jTokenDataClass = env->FindClass("net/jllama/core/LlamaTokenData");
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
    jclass jTokenDataArrayClass = env->FindClass("net/jllama/core/LlamaTokenDataArray");
    if (jTokenDataArrayClass == nullptr) {
      throw JNIException("Unable to find LlamaTokenDataArray class");
    }
    jfieldID jArrayFieldId = env->GetFieldID(jTokenDataArrayClass, "data", "[Lnet/jllama/core/LlamaTokenData;");
    if (jArrayFieldId == nullptr) {
      throw JNIException("Unable to find LlamaTokenData array \"data\"");
    }

    jclass jTokenDataClass = env->FindClass("net/jllama/core/LlamaTokenData");
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


  jintArray newJIntArray(JNIEnv* env, jint size) {
    jintArray newArray = env->NewIntArray(size);
    jthrowable const jException = env->ExceptionOccurred();
    if (jException) {
      std::ostringstream errorMessage;
      errorMessage << "Failed to create new int[] array with size=" << size << ".";
      throw JNIException(errorMessage.str().c_str(), jException);
    }
    if (!newArray) {
      std::ostringstream errorMessage;
      errorMessage << "Failed to create new int[] array with size=" << size << ", reason unknown.";
      throw JNIException("Failed to create new int[] array, reason unknown.");
    }
    return newArray;
  }


  template <typename T>
  T newPrimitiveArray(JNIEnv* env, jint size) {
    static_assert(std::is_base_of<_jarray, T>::value,
                  "T must be a Java primitive array subclass");
    static_assert(std::is_same<T, jobjectArray>::value,
                  "T must be a Java primitive array subclass");
    static_assert(std::is_same<T, _jarray*>::value,
                  "T must be a Java primitive array subclass");
    throw JNIException(
        "Unsupported type, typeName= " + std::string(typeid(T).name())
            + ". This is likely a bug in the code.");
  }

}
