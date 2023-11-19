#include <cstring>
#include <jni.h>
#include "Utf8StringManager.h"

Utf8StringManager::Utf8StringManager(JNIEnv* env, jbyteArray jutf8String) {
  jbyte* bytes = env->GetByteArrayElements(jutf8String, nullptr);
  jsize length = env->GetArrayLength(jutf8String);
  utf8String = new char[length + 1];
  memcpy(utf8String, bytes, length);
  utf8String[length] = '\0';
}

Utf8StringManager::~Utf8StringManager() {
  delete utf8String;
}

char* Utf8StringManager::getString() {
  return utf8String;
}
