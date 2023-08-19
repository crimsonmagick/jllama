package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaManagerJNIImpl implements LlamaManager {

  @Override
  public native void loadLibrary();

  @Override
  public native void closeLibrary();

  @Override
  public native void llamaBackendInit(boolean useNuma);

  @Override
  public native void llamaBackendFree();

  @Override
  public native LlamaOpaqueModel llamaLoadModelFromFile(byte[] pathModel,
      LlamaContextParams params);

  @Override
  public native LlamaOpaqueContext llamaLoadContextWithModel(LlamaOpaqueModel opaqueModel, LlamaContextParams params);

  @Override
  public native int llamaTokenizeWithModel(LlamaOpaqueModel model, byte[] text, int[] tokens, int nMaxTokens, boolean addBos);

  @Override
  public native int llamaEval(LlamaOpaqueContext context, int[] tokens, int nTokens, int nPast, int nThreads);

  @Override
  public native float[] llamaGetLogits(LlamaOpaqueContext context);

  @Override
  public native int llamaSampleTokenGreedy(LlamaOpaqueContext context, LlamaTokenDataArray candidates);

  @Override
  public native byte[] llamaTokenToStr(LlamaOpaqueContext context, int llamaToken);

}
