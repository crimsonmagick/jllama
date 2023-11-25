#include "net_jllama_llama_cpp_java_bindings_LlamaModel.h"
#include "handler/LlamaManager.h"

extern "C" {

  JNIEXPORT jobject
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaModel_newContext
      (JNIEnv* env, jobject jModel, jobject jContextParams) {
    return LlamaManager::getLlamaManager(env)
        ->newSession(env).loadContextWithModel(jModel, jContextParams);
  }

}
