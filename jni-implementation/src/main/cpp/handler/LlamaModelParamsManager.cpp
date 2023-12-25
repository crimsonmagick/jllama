#include <jni.h>
#include "../jni.h"
#include "../j_classes_consts.h"
#include "LlamaManager.h"

llama_model_params
LlamaManager::LlamaSession::LlamaModelParamsManager::getParams() {
  return llamaModelParams;
}

jobject LlamaManager::LlamaSession::LlamaModelParamsManager::getJavaParams() {
  return jLlamaModelParams;
}

LlamaManager::LlamaSession::LlamaModelParamsManager::LlamaModelParamsManager(
    llama_model_params modelParams,
    LlamaManager::LlamaSession* session)
    : llamaModelParams(modelParams), session(session) {

  JNIEnv* env = session->env;
  jclass jParamsClass = env->FindClass(JAVA_MODEL_PARAMS_NAME);
  jmethodID constructor = env->GetMethodID(jParamsClass, "<init>", "()V");
  jLlamaModelParams = env->NewObject(jParamsClass, constructor);


  jni::setSignedInt32(llamaModelParams.n_gpu_layers,
                        env,
                        jParamsClass,
                        jLlamaModelParams,
                        "nGpuLayers");
  jni::setSignedInt32(llamaModelParams.main_gpu,
                        env,
                        jParamsClass,
                        jLlamaModelParams,
                        "mainGpu");
  jni::setBoolean(llamaModelParams.vocab_only,
                  env,
                  jParamsClass,
                  jLlamaModelParams,
                  "vocabOnly");
  jni::setBoolean(llamaModelParams.use_mmap,
                  env,
                  jParamsClass,
                  jLlamaModelParams,
                  "useMmap");
  jni::setBoolean(llamaModelParams.use_mlock,
                  env,
                  jParamsClass,
                  jLlamaModelParams,
                  "useMlock");

  // IMPORTANT - not currently bothering to map any of the pointer fields
  tensorSplit = nullptr;
}

LlamaManager::LlamaSession::LlamaModelParamsManager::LlamaModelParamsManager(
    jobject javaContextParams,
    LlamaSession* session)
    : jLlamaModelParams(javaContextParams), session(session) {

  JNIEnv* env = session->env;
  jclass javaParamsClass = env->GetObjectClass(javaContextParams);

  tensorSplitFloatArray = jni::getFloatArray(env,
                                             javaParamsClass,
                                             javaContextParams,
                                             "tensorSplit");
  tensorSplit = tensorSplitFloatArray ? env->GetFloatArrayElements(tensorSplitFloatArray, nullptr) : nullptr;

  llamaModelParams = {
      .n_gpu_layers = jni::getInt32(env, javaParamsClass, javaContextParams, "nGpuLayers"),
      .main_gpu = jni::getInt32(env, javaParamsClass, javaContextParams, "mainGpu"),
      .tensor_split = tensorSplit,
      .vocab_only = jni::getBool(env, javaParamsClass, javaContextParams, "vocabOnly"),
      .use_mmap = jni::getBool(env, javaParamsClass, javaContextParams, "useMmap"),
      .use_mlock = jni::getBool(env, javaParamsClass, javaContextParams, "useMlock")
  };

  jfieldID callbackFieldId = env->GetFieldID( javaParamsClass, "progressCallback", "Ljava/util/function/Consumer;");
  if (callbackFieldId == nullptr) {
    throw jni::JNIException("field \"progressCallback\" must exist on Java LlamaModelParams class");
  }
  jobject jprogressCallback = env->GetObjectField(javaContextParams, callbackFieldId);

  CallbackContext* callbackContext;

  if (jprogressCallback) {
    callbackContext = new CallbackContext{
      env->NewGlobalRef(jprogressCallback)
    };
    llamaModelParams.progress_callback = progressCallback,
    llamaModelParams.progress_callback_user_data = callbackContext,
    env->DeleteLocalRef(jprogressCallback);
  } else {
    callbackContext = nullptr;
  }

}

LlamaManager::LlamaSession::LlamaModelParamsManager::~LlamaModelParamsManager() {
  if (tensorSplit) {
    session->env->ReleaseFloatArrayElements(tensorSplitFloatArray,
                                   (jfloat*) tensorSplit,
                                   JNI_ABORT);
  }
}
