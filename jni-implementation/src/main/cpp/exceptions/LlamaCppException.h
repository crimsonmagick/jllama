#ifndef LLAMA_CPP_LLAMACPPEXCEPTION_H
#define LLAMA_CPP_LLAMACPPEXCEPTION_H

#include <stdexcept>
#include <string>

class LlamaCppException: public std::runtime_error {

 public:
  explicit LlamaCppException(const char* message) : runtime_error(message) {

  }
};


#endif //LLAMA_CPP_LLAMACPPEXCEPTION_H
