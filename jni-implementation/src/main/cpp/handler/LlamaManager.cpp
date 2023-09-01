#include <mutex>
#include "../exceptions/LlamaCppException.h"
#include "LlamaManager.h"

LlamaManager* LlamaManager::singleton = nullptr;
JavaVM* LlamaManager::javaVm = nullptr;
jobject LlamaManager::jloggerCallback = nullptr;

LlamaManager::LlamaManager() = default;

LlamaManager* LlamaManager::getLlamaManager(JNIEnv* env) {
  JavaVM* vm;
  env->GetJavaVM(&vm);

  std::call_once(initFlag, [vm](){
    LlamaManager::javaVm = vm;
    singleton = new LlamaManager();
  });

  if (vm != javaVm) {
    throw LlamaCppException("JavaVM for provided JNIEnv does not match previously cached JavaVM. LlamaManager only supports one JavaVM per process.");
  }
  return singleton;
}
LlamaManager::LlamaSession LlamaManager::newSession(JNIEnv* env) {
  return LlamaSession(env, this);
}
