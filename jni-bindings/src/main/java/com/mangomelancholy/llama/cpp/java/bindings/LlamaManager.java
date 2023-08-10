package com.mangomelancholy.llama.cpp.java.bindings;

public interface LlamaManager {

  void loadLibrary();
  void closeLibrary();

  void llamaBackendInit(boolean useNuma);
  void llamaBackendFree();
  Object llamaLoadModelFromFile(byte[] pathModel, LlamaContextParams params);
  byte[] llamaTimesC(byte[] path);
  byte[] llamaTimesCpp(byte[] path);
}
