#ifndef JNI_IMPLEMENTATION_LLAMACONTEXTPARAMSMANAGER_H
#define JNI_IMPLEMENTATION_LLAMACONTEXTPARAMSMANAGER_H

#include "llama.h"

class LlamaContextParamsManager {
  public:
  LlamaContextParamsManager(JNIEnv* env, jobject javaContextParams, llama_progress_callback, void* callbackContext);
  ~LlamaContextParamsManager();
  llama_context_params getParams();

  private:
  JNIEnv* env;
  llama_context_params llamaContextParams{};
  jfloatArray tensorSplitFloatArray;
  const float* tensorSplit;

};


#endif //JNI_IMPLEMENTATION_LLAMACONTEXTPARAMSMANAGER_H
