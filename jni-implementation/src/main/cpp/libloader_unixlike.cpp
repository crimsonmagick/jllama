#if defined(__unix__) || defined(__APPLE__)
#include "libloader.h"

#include <dlfcn.h>
#include <sstream>
#include <iostream>
#include "exceptions/dynamic-library-exception.h"

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

void closeLibrary() {
  // TODO what if freeing the library fails?
  dlclose(llamaHandle);
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
    std::cerr << errorMessage << std::endl;
    throw DynamicLibraryException(errorMessage.c_str());
  }
  std::cout << "loaded function successfully!" << std::endl;
  return func;
}

#endif
