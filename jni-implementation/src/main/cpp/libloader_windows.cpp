#ifdef _WIN32
#include "libloader.h"

#include <iostream>
#include <sstream>
#include <windows.h>
#include "exceptions/dynamic-library-exception.h"

#define LIBRARY_SUFFIX ".dll"

HINSTANCE llamaHandle = NULL;

void loadLibrary(const std::string libName) {
  const std::string libraryFileName = libName + LIBRARY_SUFFIX;
  llamaHandle = LoadLibrary(libraryFileName.c_str());

  if (!llamaHandle) {
    std::stringstream errorMessageStream;
    errorMessageStream
        << "Could not load the dynamic library with libraryFileName="
        << libraryFileName << "error=" << GetLastError();
    std::string errorMessage = errorMessageStream.str();
    std::cerr << errorMessage << std::endl;
    throw DynamicLibraryException(errorMessage.c_str());
  }
}

void closeLibrary() {
  // TODO what if freeing the library fails?
  FreeLibrary(llamaHandle);
}

void *getFunctionAddress(const std::string functionName) {
  void *func = GetProcAddress(llamaHandle, functionName.c_str());

  if (!func) {
    std::stringstream errorMessageStream;
    errorMessageStream << "Could not locate the function with functionName="
                       << functionName << "error=" << GetLastError();
    std::string errorMessage = errorMessageStream.str();
    std::cerr << errorMessage << std::endl;
    throw DynamicLibraryException(errorMessage.c_str());
  }
  std::cout << "loaded function successfully!" << std::endl;
  return func;
}
#endif
