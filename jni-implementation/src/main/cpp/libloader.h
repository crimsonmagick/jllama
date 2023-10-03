#include <string>
#include <jni.h>

void loadLibrary(std::string libname);
void loadLibrary(std::string libname, JNIEnv* env);
void closeLibrary();
void* getFunctionAddress(std::string functionName);