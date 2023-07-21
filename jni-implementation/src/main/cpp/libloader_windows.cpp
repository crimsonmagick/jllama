#ifdef _WIN32
#include <cstdio>
#include <iostream>
#include <windows.h>
#include <format>
#include "libloader.h"
#include "exceptions/dynamic-library-exception.h"

#define LIBRARY_SUFFIX ".dll"

HINSTANCE llamaHandle = NULL;

void loadLibrary(const std::string libName) {
  const std::string libraryFileName = libName + LIBRARY_SUFFIX;
  llamaHandle = LoadLibrary(libraryFileName.c_str());

  if (!llamaHandle) {
    std::string errorMessage = std::format("could not load the dynamic library, error: {}", GetLastError());
    std::cout << errorMessage;
    throw DynamicLibraryException(errorMessage.c_str());
  }
}

void closeLibrary() {
  FreeLibrary(llamaHandle);
}

void* getFunctionAddress(const std::string functionName) {
  void * func = GetProcAddress(llamaHandle, functionName.c_str());
  if (!func) {
    printf("could not locate the function\n");
    // TODO throw exception
    return NULL;
  }
  printf("loaded function successfully!\n");
  return func;
}
#endif