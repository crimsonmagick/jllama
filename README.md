# llama-cpp-java-bindings

This is a WIP that will provide bindings for using llama.cpp with Java

The intention is to provide a basic function for function Java interface with llama.h. Once these fundamental bindings are done, fluent Java APIs will be produced for easy interaction with llama.cpp.

Currently, the testing is being done with `Main.java` in the jni-bindings project. This will be moved into proper unit and integrations tests as the project matures.

# Running the proof of concept
Make sure that you have the llama.cpp on your dynamic library path for your operating system. An example of this can be found in the top level gradle "runProgram" task. Make sure to specify the model path and your log level using: `-Dmodelpath=/path/to/model/mymodel.gguf` and `-Dloglevel={LOGLEVEL}`. Log level can be one of `OFF|INFO|WARN|ERROR`. Note that llama.cpp internal logging is very immature and currently logs a lot of info messages at the `ERROR` level.