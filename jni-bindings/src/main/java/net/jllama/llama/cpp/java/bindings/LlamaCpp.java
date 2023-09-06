package net.jllama.llama.cpp.java.bindings;

import java.util.function.BiConsumer;

public interface LlamaCpp {

  void loadLibrary();

  void closeLibrary();

  void llamaBackendInit(boolean useNuma);

  void llamaBackendFree();

  LlamaOpaqueModel llamaLoadModelFromFile(byte[] pathModel, LlamaContextParams params);

  void llamaFreeModel(LlamaOpaqueModel model);

  LlamaOpaqueContext llamaLoadContextWithModel(LlamaOpaqueModel opaqueModel, LlamaContextParams llamaContextParams);

  // free Context
  void llamaFree(LlamaOpaqueContext context);

  int llamaTokenizeWithModel(LlamaOpaqueModel model, byte[] text, int[] tokens, int nMaxTokens,
      boolean addBos);

  int llamaEval(LlamaOpaqueContext context, int[] tokens, int nTokens, int nPast, int nThreads);

  float[] llamaGetLogits(LlamaOpaqueContext context);

  int llamaSampleToken(LlamaOpaqueContext context, LlamaTokenDataArray candidates);
  int llamaSampleTokenGreedy(LlamaOpaqueContext context, LlamaTokenDataArray candidates);

  int llamaTokenToPiece(LlamaOpaqueContext context, int llamaToken, byte[] buf);

  void llamaSampleRepetitionPenalty(LlamaOpaqueContext ctx, LlamaTokenDataArray candidates, int[] lastTokens, float penalty);

  void llamaSampleFrequencyAndPresencePenalties(LlamaOpaqueContext context, LlamaTokenDataArray candidates, int[] lastTokens, float alphaFrequency, float alphaPresence);

  void llamaSampleSoftMax(LlamaOpaqueContext context, LlamaTokenDataArray candidates);

  void llamaSampleTopK(LlamaOpaqueContext context, LlamaTokenDataArray candidates, int k, long minKeep);

  int llamaTokenBos(LlamaOpaqueContext context);

  int llamaTokenEos(LlamaOpaqueContext context);

  int llamaTokenNl(LlamaOpaqueContext context);

  void llamaLogSet(BiConsumer<LlamaLogLevel, byte[]> llamaLogCallback);

  long llamaTimeUs();

  LlamaContextParams llamaContextDefaultParams();

}
