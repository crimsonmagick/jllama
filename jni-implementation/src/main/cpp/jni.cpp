#include <iostream>
#include <jni.h>
#include "exceptions/dynamic-library-exception.h"

namespace jni {
  void throwException(JNIEnv * env, DynamicLibraryException e) {
    jclass jExceptionClass = env->FindClass("com/mangomelancholy/llama/cpp/java/bindings/exceptions/DynamicLibraryException");
    if (jExceptionClass == NULL) {
      std::cerr << "ERROR - Unable to lookup Java Exception class DynamicLibraryException" << std::endl;
      return;
    }
    jmethodID constructor = env->GetMethodID(jExceptionClass, "<init>", "(Ljava/lang/String;)V");
    if (constructor == NULL) {
      std::cerr << "ERROR - Unable to lookup constructor for class DynamicLibraryException" << std::endl;
      return;
    }
    jstring jmsg = env->NewStringUTF(e.what());
    jthrowable jException = (jthrowable) env->NewObject(jExceptionClass, constructor, jmsg);
    env->Throw(jException);
  }
}
