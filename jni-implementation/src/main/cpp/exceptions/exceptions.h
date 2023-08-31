#ifndef JNI_IMPLEMENTATION_EXCEPTIONS_H
#define JNI_IMPLEMENTATION_EXCEPTIONS_H

#include <jni.h>
#include "DynamicLibraryException.h"
#include "../jni.h"
template<typename Func>
auto withJniExceptions(JNIEnv* env, Func&& func) -> decltype(func()) {
  using ReturnType = decltype(func());
  try {
    return func();
  } catch (const DynamicLibraryException &e) {
    jni::throwDLLException(env, e);
  } catch (const jni::JNIException &e) {
    jni::throwJNIException(env, e);
  } catch (const LlamaCppException &e) {
    jni::throwLlamaCppException(env, e);
  } catch (const std::exception &e) {
    jni::throwRuntimeException(env, e);
  }
  return ReturnType();
}

#endif //JNI_IMPLEMENTATION_EXCEPTIONS_H
