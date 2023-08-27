#ifndef JNI_IMPLEMENTATION_LLAMAMANAGER_H
#define JNI_IMPLEMENTATION_LLAMAMANAGER_H
#include <jni.h>
#include "llama.h"
#include "mutex"
#include "LlamaSession.h"

class LlamaManager {

  public:
  static LlamaManager* getLlamaManager(JNIEnv* env);
  llama_progress_callback getProgressCallback();
  void* generateProgressCallbackContext();
  LlamaSession newSession(JNIEnv* env);

  private:
  static LlamaManager* singleton;
  static JavaVM* javaVm;
  static inline std::once_flag initFlag;
  explicit LlamaManager();
  static void progressCallback(float progress, void *ctx);

};

#endif //JNI_IMPLEMENTATION_LLAMAMANAGER_H
