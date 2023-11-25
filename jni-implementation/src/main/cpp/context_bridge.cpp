#include "net_jllama_llama_cpp_java_bindings_LlamaContext.h"
#include "handler/LlamaManager.h"

extern "C" {
  JNIEXPORT jint
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_llamaEvalNative
      (JNIEnv* env,
       jobject jContext,
       jintArray jTokens,
       jint jnTokens,
       jint jnPast) {
    return LlamaManager::getLlamaManager(env)
      ->newSession(env).eval(jContext, jTokens, jnTokens, jnPast);
  }

  JNIEXPORT void
  JNICALL Java_net_jllama_llama_cpp_java_bindings_LlamaContext_closeNative
      (JNIEnv* env, jobject jContext) {
    LlamaManager::getLlamaManager(env)->newSession(env).freeContext(jContext);
  }
}
