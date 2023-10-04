#if defined(__unix__) || defined(__APPLE__)
#include "libloader.h"

#include <dlfcn.h>
#include <sstream>
#include <iostream>
#include "exceptions/DynamicLibraryException.h"
#include "j_classes_consts.h"
#include "jni.h"

#define LIBRARY_PREFIX "lib"

#ifdef __APPLE__
#define LIBRARY_SUFFIX ".dylib"
#else
#define LIBRARY_SUFFIX ".so"
#endif


void* llamaHandle = NULL;

void loadLibrary(std::string libName) {

  const std::string libraryFileName = LIBRARY_PREFIX + libName + LIBRARY_SUFFIX;
  llamaHandle = dlopen(libraryFileName.c_str(), RTLD_LAZY);

  if (!llamaHandle) {
    std::stringstream errorMessageStream;
    errorMessageStream
        << "Could not load the dynamic library with libraryFileName="
        << libraryFileName << "error=" << dlerror();
    std::string errorMessage = errorMessageStream.str();
    std::cerr << errorMessage << std::endl;
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
    llamaHandle = dlopen(libPath, RTLD_LAZY);
  }

  if (!llamaHandle) {
    std::stringstream stdMessageStream;
    std::string libraryFileName = libPath == nullptr ? "UNABLE_TO_RETRIEVE" : libPath;
    stdMessageStream
        << "Could not load the dynamic library with libraryFileName="
        << libraryFileName << ", error=" << dlerror();
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
    // clear any existing error
    dlerror();

    if (dlclose(llamaHandle)) {
      std::stringstream errorMessageStream;
      errorMessageStream << "dlclose(llamaHandle) failed, error=" << dlerror();
      std::string errorMessage = errorMessageStream.str();
      std::cerr << errorMessage << "\n";
      throw new DynamicLibraryException(errorMessage.c_str());
    }
}

void* getFunctionAddress(std::string functionName) {
  // clear any existing error
  dlerror();

  void* func = dlsym(llamaHandle, functionName.c_str());

  char *error = dlerror();
  if (error != NULL)  {
    std::stringstream errorMessageStream;
    errorMessageStream << "Could not locate the function with functionName="
                       << functionName << "error=" << error;
    std::string errorMessage = errorMessageStream.str();
    std::cerr << errorMessage << "\n";
    throw DynamicLibraryException(errorMessage.c_str());
  }
  return func;
}

#endif
