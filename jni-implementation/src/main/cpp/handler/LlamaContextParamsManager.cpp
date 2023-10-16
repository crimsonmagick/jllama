#include <jni.h>
#include "../jni.h"
#include "../j_classes_consts.h"
#include "LlamaManager.h"

llama_context_params
LlamaManager::LlamaSession::LlamaContextParamsManager::getParams() {
  return llamaContextParams;
}

jobject LlamaManager::LlamaSession::LlamaContextParamsManager::getJavaParams() {
  return jLlamaContextParams;
}

LlamaManager::LlamaSession::LlamaContextParamsManager::LlamaContextParamsManager(
    llama_context_params contextParams,
    LlamaManager::LlamaSession* session)
    : llamaContextParams(contextParams), session(session) {

  JNIEnv* env = session->env;
  jclass jParamsClass = env->FindClass(JAVA_CONTEXT_PARAMS_NAME);
  jmethodID constructor = env->GetMethodID(jParamsClass, "<init>", "()V");
  jLlamaContextParams = env->NewObject(jParamsClass, constructor);

  jni::setUnsignedInt32(llamaContextParams.seed,
                        env,
                        jParamsClass,
                        jLlamaContextParams,
                        "seed");
  jni::setUnsignedInt32(llamaContextParams.n_ctx,
                        env,
                        jParamsClass,
                        jLlamaContextParams,
                        "nCtx");
  jni::setUnsignedInt32(llamaContextParams.n_batch,
                        env,
                        jParamsClass,
                        jLlamaContextParams,
                        "nBatch");
  jni::setFloat(llamaContextParams.rope_freq_base,
                env,
                jParamsClass,
                jLlamaContextParams,
                "ropeFreqBase");
  jni::setFloat(llamaContextParams.rope_freq_scale,
                env,
                jParamsClass,
                jLlamaContextParams,
                "ropeFreqScale");
  jni::setBoolean(llamaContextParams.mul_mat_q,
                env,
                jParamsClass,
                jLlamaContextParams,
                "mulMatQ");
  jni::setBoolean(llamaContextParams.f16_kv,
                  env,
                  jParamsClass,
                  jLlamaContextParams,
                  "f16Kv");
  jni::setBoolean(llamaContextParams.logits_all,
                  env,
                  jParamsClass,
                  jLlamaContextParams,
                  "logitsAll");
  jni::setBoolean(llamaContextParams.embedding,
                  env,
                  jParamsClass,
                  jLlamaContextParams,
                  "embedding");
}

LlamaManager::LlamaSession::LlamaContextParamsManager::LlamaContextParamsManager(
    jobject javaContextParams,
    LlamaSession *session)
    : jLlamaContextParams(javaContextParams), session(session) {

  JNIEnv* env = session->env;
  jclass javaParamsClass = env->GetObjectClass(javaContextParams);

  llamaContextParams = {
      jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "seed"),
      jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "nCtx"),
      jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "nBatch"),
      jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "nThreads"),
      jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "nThreadsBatch"),
      jni::getFloat(env, javaParamsClass, javaContextParams, "ropeFreqBase"),
      jni::getFloat(env, javaParamsClass, javaContextParams, "ropeFreqScale"),
      jni::getBool(env, javaParamsClass, javaContextParams, "mulMatQ"),
      jni::getBool(env, javaParamsClass, javaContextParams, "f16Kv"),
      jni::getBool(env, javaParamsClass, javaContextParams, "logitsAll"),
      jni::getBool(env, javaParamsClass, javaContextParams, "embedding")
  };
}