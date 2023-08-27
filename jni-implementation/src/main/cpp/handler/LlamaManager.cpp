#include "LlamaManager.h"
#include "../exceptions/LlamaCppException.h"

#include <mutex>

LlamaManager* LlamaManager::singleton = nullptr;
JavaVM* LlamaManager::javaVm = nullptr;

LlamaManager::LlamaManager() = default;

llama_progress_callback LlamaManager::getProgressCallback() {
  return progressCallback;
}
void* LlamaManager::generateProgressCallbackContext() {
  return nullptr; // TODO would any contextual information be of interest? JavaVM is already accessible statically. We could also allow the calling Java program to pass info, but that seems unnecessary
}

void LlamaManager::progressCallback(float progress, void* ctx) {
  // TODO call back into Java using JavaVM to attach a thread and invoke a method
}

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
