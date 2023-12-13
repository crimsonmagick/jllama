#include <mutex>
#include "../exceptions/LlamaCppException.h"
#include "LlamaManager.h"

LlamaManager* LlamaManager::singleton = nullptr;
JavaVM* LlamaManager::javaVm = nullptr;
jobject LlamaManager::jloggerCallback = nullptr;

LlamaManager::LlamaManager() = default;

LlamaManager* LlamaManager::getLlamaManager(JNIEnv* env) {

  std::call_once(initFlag, [env](){
    env->GetJavaVM(&LlamaManager::javaVm);
    singleton = new LlamaManager();
  });
  return singleton;
}
LlamaManager::LlamaSession LlamaManager::newSession(JNIEnv* env) {
  return LlamaSession(env, this);
}
