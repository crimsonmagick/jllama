package com.mangomelancholy.llama.cpp.java.bindings;

public interface LlamaManager {

  void loadLibrary();
  void closeLibrary();

  void llamaBackendInit(boolean useNuma);
  void llamaBackendFree();
}
