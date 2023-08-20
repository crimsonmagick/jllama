#include "LlamaSession.h"
#include "../libloader.h"

typedef void (* llama_backend_init_pointer)(bool);

void LlamaSession::backendInit(bool useNuma) {
  withJniExceptions([&useNuma]{
    llama_backend_init_pointer func =
        (llama_backend_init_pointer) getFunctionAddress("llama_backend_init");
    func(useNuma);
  });
}
