#include <cstring>
#include <jni.h>
#include "jni.h"
#include "LlamaContextParamsManager.h"

llama_context_params LlamaContextParamsManager::getParams() {
  return llamaContextParams;
}

LlamaContextParamsManager::LlamaContextParamsManager
    (JNIEnv* env, jobject javaContextParams) :
    LlamaContextParamsManager(env, nullptr, javaContextParams) {
}

LlamaContextParamsManager::LlamaContextParamsManager
    (JNIEnv* env, jbyteArray jPath, jobject javaContextParams) : env(env) {
  jclass javaParamsClass = env->GetObjectClass(javaContextParams);

  if (jPath) {
    jbyte* pathBytes = env->GetByteArrayElements(jPath, nullptr);
    jsize length = env->GetArrayLength(jPath);
    path = new char[length + 1];
    memcpy(path, pathBytes, length);
    path[length] = '\0';
  } else {
    path = nullptr;
  }

  tensorSplitFloatArray = jni::getJFloatArray(env,
                                              javaParamsClass,
                                              javaContextParams,
                                              "tensorSplit");
  tensorSplit = tensorSplitFloatArray ?
                env->GetFloatArrayElements(tensorSplitFloatArray, nullptr) : nullptr;

  llamaContextParams = {
      jni::getUnsignedInt32(env, javaParamsClass, javaContextParams, "seed"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "nCtx"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "nBatch"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "nGqa"),
      jni::getFloat(env, javaParamsClass, javaContextParams, "rmsNormEps"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "nGpuLayers"),
      jni::getInt32(env, javaParamsClass, javaContextParams, "mainGpu"),
      tensorSplit,
      jni::getFloat(env, javaParamsClass, javaContextParams, "ropeFreqBase"),
      jni::getFloat(env, javaParamsClass, javaContextParams, "ropeFreqScale"),
      nullptr,
      nullptr,
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

LlamaContextParamsManager::~LlamaContextParamsManager() {
  delete[] path;
  if (tensorSplit) {
    env->ReleaseFloatArrayElements(tensorSplitFloatArray,
                                   (jfloat*) tensorSplit,
                                   JNI_ABORT);
  }
}
char* LlamaContextParamsManager::getPath() {
  return path;
}
