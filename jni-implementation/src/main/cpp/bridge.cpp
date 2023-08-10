#include <iostream>
#include <cstdio>
#include "com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl.h"
#include "exceptions/dynamic-library-exception.h"
#include "jni.h"
#include "libloader.h"
#include "../../../../external/llama.cpp/llama.h"

typedef void (*llama_backend_init_pointer)(bool);
typedef void (*llama_backend_free_pointer)();
typedef llama_model* (*llama_load_model_from_file_pointer)(const char*, struct llama_context_params);

extern "C" {

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_loadLibrary(
      JNIEnv *env,
      jobject thisObject) {

    try {
      loadLibrary("llama");
    } catch (const DynamicLibraryException &e) {
      jni::throwException(env, e);
    }
  }

  JNIEXPORT void
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_closeLibrary(
      JNIEnv *env,
      jobject thisObject) {
    try {
      closeLibrary();
    } catch (const DynamicLibraryException &e) {
      jni::throwException(env, e);
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
      jni::throwException(env, e);
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
      jni::throwException(env, e);
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

      jbyte *pathBytes = env->GetByteArrayElements(path, NULL);
      jsize length = env->GetArrayLength(path);
      char *llamaPath = new char[length + 1];
      memcpy(llamaPath, pathBytes, length);
      llamaPath[length] = '\n';

      jfieldID seedFieldId = env->GetFieldID(javaParamsClass, "seed", "J");
      long seed = env->GetLongField(javaParams, seedFieldId);
      // TODO marshall java params to llama_params and call method
    } catch (const DynamicLibraryException &e) {
      jni::throwException(env, e);
    }
    return (jobject) nullptr;
  }

  JNIEXPORT jbyteArray
  JNICALL Java_com_mangomelancholy_llama_cpp_java_bindings_LlamaManagerJNIImpl_llamaTimesCpp
      (JNIEnv *env, jobject thisObject, jbyteArray path) {
    jbyte *bytes = env->GetByteArrayElements(path, NULL);
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
    jbyte *bytes = env->GetByteArrayElements(path, NULL);
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
