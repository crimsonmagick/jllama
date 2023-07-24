#ifndef LLAMA_CPP_JAVA_BINDINGS_JNI_H
#define LLAMA_CPP_JAVA_BINDINGS_JNI_H
#include "exceptions/dynamic-library-exception.h"

namespace jni {
  void throwException(JNIEnv * env, DynamicLibraryException e);
}
#endif //LLAMA_CPP_JAVA_BINDINGS_JNI_H
