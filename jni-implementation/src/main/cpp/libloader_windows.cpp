#ifdef _WIN32

#include "j_classes_consts.h"
#include "jni.h"
#include "libloader.h"
#include <iostream>
#include <sstream>
#include <windows.h>
#include "exceptions/DynamicLibraryException.h"

#define LIBRARY_SUFFIX ".dll"

HINSTANCE llamaHandle = NULL;

void loadLibrary(const std::string libName) {
  const std::string libraryFileName = libName + LIBRARY_SUFFIX;
  llamaHandle = LoadLibrary(libraryFileName.c_str());

  if (!llamaHandle) {
    std::stringstream stdMessageStream;
    stdMessageStream
        << "Could not load the dynamic library with libraryFileName="
        << libraryFileName << ", error=" << GetLastError();
    std::string errorMessage = stdMessageStream.str();
    std::cout << errorMessage << "\n";
    throw DynamicLibraryException(errorMessage.c_str());
  }
}


void loadLibrary(const std::string libname, JNIEnv* env) {

  jclass dllUtil = env->FindClass(DLL_UTIL_NAME);
  if (!dllUtil) {
    std::string errorMessage = "Unable to find Java class with className="
        + std::string(DLL_UTIL_NAME);
    throw jni::JNIException(errorMessage.c_str());
  }

  jmethodID methodId = env->GetStaticMethodID(dllUtil, "extract", "(Ljava/lang/String;)Ljava/lang/String;");
  if (!methodId) {
    throw jni::JNIException("Unable to find method with methodName=extract");
  }

  jstring jlibName = env->NewStringUTF(libname.c_str());
  jstring jlibPath = static_cast<jstring>(env->CallStaticObjectMethod(dllUtil, methodId, jlibName));

  const char* libPath;
  if (env->ExceptionCheck()){
    std::cerr << "DllUtil#extract error, errorMessage=";
    env->ExceptionDescribe();
    std::cerr << std::endl;
    env->ExceptionClear();
    libPath = nullptr;
    llamaHandle = nullptr;
  } else {
    libPath = env->GetStringUTFChars(jlibPath, nullptr);
    llamaHandle = LoadLibrary(libPath);
  }

  if (!llamaHandle) {
    std::stringstream stdMessageStream;
    std::string libraryFileName = libPath == nullptr ? "UNABLE_TO_RETRIEVE" : libPath;
    stdMessageStream
        << "Could not load the dynamic library with libraryFileName="
        << libraryFileName << ", error=" << GetLastError();
    std::string errorMessage = stdMessageStream.str();
    std::cout << errorMessage << std::endl;
    if (libPath) {
      env->ReleaseStringUTFChars(jlibPath, libPath);
    }
    throw DynamicLibraryException(errorMessage.c_str());
  }

  env->ReleaseStringUTFChars(jlibPath, libPath);
}

void closeLibrary() {
  if (!FreeLibrary(llamaHandle)) {
    std::stringstream errorMessageStream;
    errorMessageStream << "FreeLibrary(llamaHandle) failed, error=" << GetLastError();
    std::string errorMessage = errorMessageStream.str();
    std::cerr << errorMessage << "\n";
    throw new DynamicLibraryException(errorMessage.c_str());
  }
}

void *getFunctionAddress(const std::string functionName) {
  void* func = reinterpret_cast<void*>(GetProcAddress(llamaHandle, functionName.c_str()));

  if (!func) {
    std::stringstream errorMessageStream;
    errorMessageStream << "Could not locate the function with functionName="
                       << functionName << ", error=" << GetLastError();
    std::string errorMessage = errorMessageStream.str();
    std::cerr << errorMessage << "\n";
    throw DynamicLibraryException(errorMessage.c_str());
  }
  return func;
}

#endif
