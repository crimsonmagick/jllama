#ifdef _WIN32
#include <cstdio>
#include <windows.h>
#include "libloader.h"

#define LIBRARY_SUFFIX ".dll"


HINSTANCE llamaHandle = NULL;

void loadLibrary(const std::string libName) {
  const std::string libraryFileName = libName + LIBRARY_SUFFIX;
  llamaHandle = LoadLibrary(libraryFileName.c_str());

  if (!llamaHandle) {
    printf("could not load the dynamic library, error: %d\n", GetLastError());
    // TODO throw exception instead
    return;
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