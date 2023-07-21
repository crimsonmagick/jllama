#if defined(__unix__) || defined(__APPLE__)

#include <cstdio>
#include "libloader.h"

#define LIBRARY_PREFIX "lib"

#ifdef __APPLE__
#define LIBRARY_SUFFIX ".dylib"
#else
#define LIBRARY_SUFFIX ".so"
#endif

#include <dlfcn.h>

void* llamaHandle = NULL;

void loadLibrary(std::string libName) {

  const std::string libraryFileName = LIBRARY_PREFIX + libName + LIBRARY_SUFFIX;
  llamaHandle = dlopen(libraryFileName.c_str(), RTLD_LAZY);

  if (!llamaHandle) {
    printf("could not load the dynamic library, error: %s\n", dlerror());
    return;
  }

}

void closeLibrary() {
  dlclose(llamaHandle);
}

void* getFunctionAddress(std::string functionName) {
  // clear any existing error
  dlerror();

  void* func = dlsym(llamaHandle, functionName.c_str());

  char *error = dlerror();
  if (error != NULL)  {
    printf("could not locate the function, error: %s\n", error);
    return NULL;
  }
  printf("loaded function successfully!");
  return func;
}

#endif