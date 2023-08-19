package com.mangomelancholy.llama.cpp.java.bindings;

public interface LlamaManager {

  void loadLibrary();

  void closeLibrary();

  void llamaBackendInit(boolean useNuma);

  void llamaBackendFree();

  LlamaOpaqueModel llamaLoadModelFromFile(byte[] pathModel, LlamaContextParams params);

  LlamaOpaqueContext llamaLoadContextWithModel(LlamaOpaqueModel opaqueModel,
      LlamaContextParams llamaContextParams);

  int llamaTokenizeWithModel(LlamaOpaqueModel model, byte[] text, int[] tokens, int nMaxTokens,
      boolean addBos);

  int llamaEval(LlamaOpaqueContext context, int[] tokens, int nTokens, int nPast, int nThreads);

  float[] llamaGetLogits(LlamaOpaqueContext context);

  int llamaSampleTokenGreedy(LlamaOpaqueContext context, LlamaTokenDataArray candidates);

  byte[] llamaTokenToStr(LlamaOpaqueContext context, int llamaToken);
}
