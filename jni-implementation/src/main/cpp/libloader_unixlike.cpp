#if defined(__unix__) || defined(__APPLE__)
#include "libloader.h"

#include <dlfcn.h>
#include <sstream>
#include <iostream>
#include "exceptions/DynamicLibraryException.h"

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
    std::cerr << errorMessage << "\n";
    throw DynamicLibraryException(errorMessage.c_str());
  }
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
  std::cout << "loaded function successfully!\n";
  return func;
}

#endif
