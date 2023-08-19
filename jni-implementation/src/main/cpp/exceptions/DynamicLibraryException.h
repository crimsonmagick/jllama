#ifndef LLAMA_CPP_DYNAMICLIBRARYEXCEPTION_H
#define LLAMA_CPP_DYNAMICLIBRARYEXCEPTION_H

#include <stdexcept>
#include <string>

class DynamicLibraryException: public std::runtime_error {

 public:
  explicit DynamicLibraryException(const char* message) : runtime_error(message) {

  }
};


#endif //LLAMA_CPP_DYNAMICLIBRARYEXCEPTION_H
