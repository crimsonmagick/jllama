#ifndef LLAMA_CPP_DYNAMICLIBRARYEXCEPTION_H
#define LLAMA_CPP_DYNAMICLIBRARYEXCEPTION_H

#include <exception>
#include <string>

class DynamicLibraryException: public std::exception {

 public:
  explicit DynamicLibraryException(const char* message) : exception(message) {

  }
};


#endif //LLAMA_CPP_DYNAMICLIBRARYEXCEPTION_H
