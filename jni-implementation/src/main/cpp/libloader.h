#include <string>

void loadLibrary(std::string libname);
void closeLibrary();
void* getFunctionAddress(std::string functionName);