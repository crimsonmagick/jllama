#include <iostream>
#include <cstdio>
#include "llama.h"
#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl.h"
#include "exceptions/dynamic-library-exception.h"
#include "jni.h"
#include "libloader.h"

typedef void (*llama_backend_init_pointer)(bool);
typedef void (*llama_backend_free_pointer)();
typedef llama_model *(*llama_load_model_from_file_pointer)
    (const char *, struct llama_context_params);

extern "C" {

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_loadLibrary(
      JNIEnv *env,
      jobject thisObject) {

    try {
      loadLibrary("llama");
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_closeLibrary(
      JNIEnv *env,
      jobject thisObject) {
    try {
      closeLibrary();
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaBackendInit(
      JNIEnv *env,
      jobject thisObject,
      jboolean useNuma) {
    try {
      llama_backend_init_pointer func =
          (llama_backend_init_pointer) getFunctionAddress("llama_backend_init");
      func(useNuma);
      std::cout << "Initialized llama.cpp backend\n";
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaBackendFree(
      JNIEnv *env,
      jobject thisObject) {
    try {
      llama_backend_free_pointer llamaFree =
          (llama_backend_free_pointer) getFunctionAddress("llama_backend_free");
      llamaFree();
      std::cout << "llama freed\n";
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    }
  }

  JNIEXPORT jobject
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaLoadModelFromFile(
      JNIEnv *env,
      jobject thisObject,
      jbyteArray path,
      jobject javaParams) {

    try {
      llama_load_model_from_file_pointer llamaLoadModel =
          (llama_load_model_from_file_pointer) getFunctionAddress(
              "llama_load_model_from_file");
      jclass javaParamsClass = env->GetObjectClass(javaParams);

      jbyte *pathBytes = env->GetByteArrayElements(path, nullptr);
      jsize length = env->GetArrayLength(path);
      char *llamaPath = new char[length + 1];
      memcpy(llamaPath, pathBytes, length);
      llamaPath[length] = '\0';

      jfloatArray floatArray = jni::getJFloatArray(env, javaParamsClass, javaParams, "tensorSplit");
      const float * tensorSplit = floatArray ? env->GetFloatArrayElements(floatArray, nullptr) : nullptr;
      llama_context_params contextParams = {
          jni::getUnsignedInt32(env, javaParamsClass, javaParams, "seed"),
          jni::getInt32(env, javaParamsClass, javaParams, "nCtx"),
          jni::getInt32(env, javaParamsClass, javaParams, "nBatch"),
          jni::getInt32(env, javaParamsClass, javaParams, "nGqa"),
          jni::getFloat(env, javaParamsClass, javaParams, "rmsNormEps"),
          jni::getInt32(env, javaParamsClass, javaParams, "nGpuLayers"),
          jni::getInt32(env, javaParamsClass, javaParams, "mainGpu"),
          tensorSplit,
          jni::getFloat(env, javaParamsClass, javaParams, "ropeFreqBase"),
          jni::getFloat(env, javaParamsClass, javaParams, "ropeFreqScale"),
          nullptr,
          nullptr,
          jni::getBool(env, javaParamsClass, javaParams, "lowVram"),
          jni::getBool(env, javaParamsClass, javaParams, "mulMatQ"),
          jni::getBool(env, javaParamsClass, javaParams, "f16Kv"),
          jni::getBool(env, javaParamsClass, javaParams, "logitsAll"),
          jni::getBool(env, javaParamsClass, javaParams, "vocabOnly"),
          jni::getBool(env, javaParamsClass, javaParams, "useMmap"),
          jni::getBool(env, javaParamsClass, javaParams, "useMlock"),
          jni::getBool(env, javaParamsClass, javaParams, "embedding")
      };

      llama_model* model = llamaLoadModel(llamaPath, contextParams);

      delete[] llamaPath;
      if (tensorSplit) {
        env->ReleaseFloatArrayElements(floatArray, (jfloat*)tensorSplit, JNI_ABORT);
      }

      if (!model) {
        jni::throwJNIException(env, jni::JNIException("Unable to load llama model from file"));
        return nullptr;
      } else {
        return jni::constructLlamaOpaqueModel(env, model);
      }
    } catch (const DynamicLibraryException &e) {
      jni::throwDLLException(env, e);
    } catch (const jni::JNIException &e) {
      jni::throwJNIException(env, e);
    }
  }

  JNIEXPORT jbyteArray
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaTimesCpp
      (JNIEnv *env, jobject thisObject, jbyteArray path) {
    jbyte *bytes = env->GetByteArrayElements(path, nullptr);
    jsize len = env->GetArrayLength(path);

    std::string cppStr(reinterpret_cast<char *>(bytes), len);
    std::cout << cppStr << std::endl;

    jbyteArray result = env->NewByteArray(len);
    env->SetByteArrayRegion(result,
                            0,
                            len,
                            reinterpret_cast<jbyte *>(cppStr.data()));

    env->ReleaseByteArrayElements(path, bytes, JNI_ABORT);

    return result;;
  }

  JNIEXPORT jbyteArray
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaTimesC
      (JNIEnv *env, jobject thisObject, jbyteArray path) {
    jbyte *bytes = env->GetByteArrayElements(path, nullptr);
    jsize len = env->GetArrayLength(path);
    char *cStr = new char[len + 1];
    memcpy(cStr, bytes, len);
    cStr[len] = '\n';

    fwrite(cStr, 1, len + 1, stdout);
    fflush(stdout);

    jbyteArray result = env->NewByteArray(len);
    env->SetByteArrayRegion(result,
                            0,
                            len,
                            reinterpret_cast<const jbyte *>(cStr));

    delete[] cStr;
    env->ReleaseByteArrayElements(path, bytes, JNI_ABORT);

    return result;
  }
}
