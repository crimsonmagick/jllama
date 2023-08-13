#ifndef JNI_IMPLEMENTATION_LLAMACONTEXTPARAMSMANAGER_H
#define JNI_IMPLEMENTATION_LLAMACONTEXTPARAMSMANAGER_H

#include "llama.h"

class LlamaContextParamsManager {
  public:
  LlamaContextParamsManager(JNIEnv* env, jbyteArray jPath,
                                     jobject javaContextParams);
  LlamaContextParamsManager(JNIEnv* env, jobject javaContextParams);
  ~LlamaContextParamsManager();
  char* getPath();
  llama_context_params getParams();

  private:
  JNIEnv* env;
  char* path;
  llama_context_params llamaContextParams{};
  jfloatArray tensorSplitFloatArray;
  const float* tensorSplit;

};


#endif //JNI_IMPLEMENTATION_LLAMACONTEXTPARAMSMANAGER_H
