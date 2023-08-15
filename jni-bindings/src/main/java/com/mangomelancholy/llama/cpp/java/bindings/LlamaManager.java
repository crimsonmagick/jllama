package com.mangomelancholy.llama.cpp.java.bindings;

public interface LlamaManager {
  void loadLibrary();
  void closeLibrary();
  void llamaBackendInit(boolean useNuma);
  void llamaBackendFree();
  LlamaOpaqueModel llamaLoadModelFromFile(byte[] pathModel, LlamaContextParams params);
  LlamaOpaqueContext llamaLoadContextWithModel(LlamaOpaqueModel opaqueModel, LlamaContextParams llamaContextParams);
  int llamaTokenizeWithModel(LlamaOpaqueModel model, byte[] text, int[] tokens, int nMaxTokens, boolean addBos);
}