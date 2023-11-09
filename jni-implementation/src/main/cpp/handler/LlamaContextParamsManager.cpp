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
      .seed = jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "seed"),
      .n_ctx = jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "nCtx"),
      .n_batch = jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "nBatch"),
      .n_threads = jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "nThreads"),
      .n_threads_batch = jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "nThreadsBatch"),
      .rope_freq_base = jni::getFloat(env, javaParamsClass, javaContextParams, "ropeFreqBase"),
      .rope_freq_scale = jni::getFloat(env, javaParamsClass, javaContextParams, "ropeFreqScale"),
      .mul_mat_q = jni::getBool(env, javaParamsClass, javaContextParams, "mulMatQ"),
      .f16_kv = jni::getBool(env, javaParamsClass, javaContextParams, "f16Kv"),
      .logits_all = jni::getBool(env, javaParamsClass, javaContextParams, "logitsAll"),
      .embedding = jni::getBool(env, javaParamsClass, javaContextParams, "embedding")
  };
}