package com.mangomelancholy.llama.cpp.java.bindings;

import java.util.function.BiConsumer;

class LlamaCppJNIImpl implements LlamaCpp {

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
  public native void llamaFreeModel(LlamaOpaqueModel model);

  @Override
  public native LlamaOpaqueContext llamaLoadContextWithModel(LlamaOpaqueModel opaqueModel,
      LlamaContextParams params);

  @Override
  public native void llamaFree(LlamaOpaqueContext context);

  @Override
  public native int llamaTokenizeWithModel(LlamaOpaqueModel model, byte[] text, int[] tokens,
      int nMaxTokens, boolean addBos);

  @Override
  public native int llamaEval(LlamaOpaqueContext context, int[] tokens, int nTokens, int nPast,
      int nThreads);

  @Override
  public native float[] llamaGetLogits(LlamaOpaqueContext context);

  @Override
  public native int llamaSampleTokenGreedy(LlamaOpaqueContext context,
      LlamaTokenDataArray candidates);

  @Override
  public native int llamaTokenToPiece(LlamaOpaqueContext context, int llamaToken, byte[] buf);

  @Override
  public native int llamaTokenBos(LlamaOpaqueContext context);

  @Override
  public native int llamaTokenEos(LlamaOpaqueContext context);

  @Override
  public native int llamaTokenNl(LlamaOpaqueContext context);

  @Override
  public native void llamaLogSet(BiConsumer<LlamaLogLevel, byte[]> llamaLogCallback);

  @Override
  public native long llamaTimeUs();

}
