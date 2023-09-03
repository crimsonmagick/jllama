#include <jni.h>
#include "../jni.h"
#include "../j_classes_consts.h"
#include "LlamaManager.h"

llama_context_params
LlamaManager::LlamaSession::LlamaContextParamsManager::getParams() {
  return llamaContextParams;
}

jobject LlamaManager::LlamaSession::LlamaContextParamsManager::getJavaPrams() {
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
  jni::setSignedInt32(llamaContextParams.n_ctx,
                        env,
                        jParamsClass,
                        jLlamaContextParams,
                        "nCtx");
  jni::setSignedInt32(llamaContextParams.n_batch,
                        env,
                        jParamsClass,
                        jLlamaContextParams,
                        "nBatch");
  jni::setSignedInt32(llamaContextParams.n_gpu_layers,
                        env,
                        jParamsClass,
                        jLlamaContextParams,
                        "nGpuLayers");
  jni::setSignedInt32(llamaContextParams.main_gpu,
                        env,
                        jParamsClass,
                        jLlamaContextParams,
                        "mainGpu");

  jni::setFloat(llamaContextParams.rope_freq_base,
                env,
                jParamsClass,
                jLlamaContextParams,
                "ropeFreqBase");

  jni::setBoolean(llamaContextParams.low_vram,
                env,
                jParamsClass,
                jLlamaContextParams,
                "lowVram");

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

  jni::setBoolean(llamaContextParams.vocab_only,
                  env,
                  jParamsClass,
                  jLlamaContextParams,
                  "vocabOnly");

  jni::setBoolean(llamaContextParams.use_mmap,
                  env,
                  jParamsClass,
                  jLlamaContextParams,
                  "useMmap");


  jni::setBoolean(llamaContextParams.use_mlock,
                  env,
                  jParamsClass,
                  jLlamaContextParams,
                  "useMlock");

  jni::setBoolean(llamaContextParams.embedding,
                  env,
                  jParamsClass,
                  jLlamaContextParams,
                  "embedding");

  // IMPORTANT - not currently bothering to map any of the pointer fields
  tensorSplit = nullptr;
}

LlamaManager::LlamaSession::LlamaContextParamsManager::LlamaContextParamsManager(
    jobject javaContextParams,
    LlamaSession* session)
    : jLlamaContextParams(javaContextParams), session(session) {

  JNIEnv* env = session->env;
  jclass javaParamsClass = env->GetObjectClass(javaContextParams);

  tensorSplitFloatArray = jni::getJFloatArray(env,
                                              javaParamsClass,
                                              javaContextParams,
                                              "tensorSplit");
  tensorSplit = tensorSplitFloatArray ? env->GetFloatArrayElements(tensorSplitFloatArray, nullptr) : nullptr;

  jfieldID callbackFieldId = env->GetFieldID( javaParamsClass, "progressCallback", "Ljava/util/function/Consumer;");
  if (callbackFieldId == nullptr) {
    throw jni::JNIException("field \"progressCallback\" must exist on Java LlamaContextParams class");
  }
  jobject jprogressCallback = env->GetObjectField(javaContextParams, callbackFieldId);

  CallbackContext* callbackContext;

  if (jprogressCallback) {
    callbackContext = new CallbackContext{
      env->NewGlobalRef(jprogressCallback)
    };
    env->DeleteLocalRef(jprogressCallback);
  } else {
    callbackContext = nullptr;
  }

  llamaContextParams = {
      jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "seed"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "nCtx"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "nBatch"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "nGpuLayers"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "mainGpu"),
      tensorSplit,
      jni::getFloat(env, javaParamsClass, javaContextParams, "ropeFreqBase"),
      jni::getFloat(env, javaParamsClass, javaContextParams, "ropeFreqScale"),
      progressCallback,
      callbackContext,
      jni::getBool(env, javaParamsClass, javaContextParams, "lowVram"),
      jni::getBool(env, javaParamsClass, javaContextParams, "mulMatQ"),
      jni::getBool(env, javaParamsClass, javaContextParams, "f16Kv"),
      jni::getBool(env, javaParamsClass, javaContextParams, "logitsAll"),
      jni::getBool(env, javaParamsClass, javaContextParams, "vocabOnly"),
      jni::getBool(env, javaParamsClass, javaContextParams, "useMmap"),
      jni::getBool(env, javaParamsClass, javaContextParams, "useMlock"),
      jni::getBool(env, javaParamsClass, javaContextParams, "embedding")
  };
}

LlamaManager::LlamaSession::LlamaContextParamsManager::~LlamaContextParamsManager() {
  if (tensorSplit) {
    session->env->ReleaseFloatArrayElements(tensorSplitFloatArray,
                                   (jfloat*) tensorSplit,
                                   JNI_ABORT);
  }
}
