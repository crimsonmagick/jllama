#include <jni.h>
#include <iostream>
#include "net_jllama_core_LlamaCpp.h"
#include "exceptions/exceptions.h"
#include "handler/LlamaManager.h"
#include "libloader.h"

LlamaManager* llamaManager = nullptr;

extern "C" {

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaCpp_loadLibrary(
      JNIEnv *env, jclass classReference) {

    withJniExceptions(env, [env] {
      const char* libraryName = "llama";
      try {
        loadLibrary(libraryName);
      } catch (const DynamicLibraryException& e) {
        std::cout << "Unable to load library based on environment variables. Attempting to load from Java resources" << std::endl;
        try {
          loadLibrary(libraryName, env);
        } catch (const DynamicLibraryException& e) {
          std::cout << "Unable to load library from Java resources" << std::endl;
          jni::throwDLLException(env, e);
        }
      }
      if (!llamaManager) {
        llamaManager = LlamaManager::getLlamaManager(env);
      }
    });
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaCpp_closeLibrary(
      JNIEnv *env,
      jclass classReference) {
      withJniExceptions(env, [] {
        closeLibrary();
      });
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaCpp_llamaBackendInit(
      JNIEnv *env,
      jclass classReference,
      jboolean useNuma) {
    llamaManager->newSession(env).backendInit(useNuma);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_core_LlamaCpp_llamaBackendFree(JNIEnv *env, jclass classReference) {
    llamaManager->newSession(env).backendFree();
  }

  JNIEXPORT jobject
  JNICALL Java_net_jllama_core_LlamaCpp_loadModel(
      JNIEnv *env, jclass classReference, jbyteArray path, jobject javaParams) {
    return llamaManager->newSession(env).loadModelFromFile(path, javaParams);
  }

  JNIEXPORT void JNICALL Java_net_jllama_core_LlamaCpp_llamaLogSet(JNIEnv* env, jclass classReference, jobject llamaLogCallback) {
    return llamaManager->newSession(env).setLogger(llamaLogCallback);
  }

  JNIEXPORT jlong JNICALL Java_net_jllama_core_LlamaCpp_llamaTimeUs(JNIEnv* env, jclass classReference) {
    return llamaManager->newSession(env).getTimestampInMicroseconds();
  }

}
