package net.jllama.llama.cpp.java.bindings;

import java.util.function.BiConsumer;

public interface LlamaCpp {

  void loadLibrary();

  void closeLibrary();

  void llamaBackendInit(boolean useNuma);

  void llamaBackendFree();

  LlamaOpaqueModel llamaLoadModelFromFile(byte[] pathModel, LlamaModelParams params);

  void llamaFreeModel(LlamaOpaqueModel model);

  LlamaContext llamaNewContextWithModel(LlamaOpaqueModel opaqueModel, LlamaContextParams llamaContextParams);

  // free Context
  void llamaFree(LlamaContext context);

  int llamaTokenize(LlamaOpaqueModel model, byte[] text, int[] tokens, int nMaxTokens, boolean addBos);

  int llamaEval(LlamaContext context, int[] tokens, int nTokens, int nPast);

  float[] llamaGetLogits(LlamaContext context);

  int llamaSampleToken(LlamaContext context, LlamaTokenDataArray candidates);
  int llamaSampleTokenGreedy(LlamaContext context, LlamaTokenDataArray candidates);

  int llamaTokenToPiece(LlamaOpaqueModel model, int llamaToken, byte[] buf);

  void llamaSampleRepetitionPenalty(LlamaContext ctx, LlamaTokenDataArray candidates, int[] lastTokens, float penalty);

  void llamaSampleFrequencyAndPresencePenalties(LlamaContext context, LlamaTokenDataArray candidates, int[] lastTokens, float alphaFrequency, float alphaPresence);

  void llamaSampleSoftMax(LlamaContext context, LlamaTokenDataArray candidates);

  void llamaSampleTopK(LlamaContext context, LlamaTokenDataArray candidates, int k, long minKeep);

  void llamaSampleTopP(LlamaContext context, LlamaTokenDataArray candidates, float p, long minKeep);

  void llamaSampleTailFree(LlamaContext context, LlamaTokenDataArray candidates, float z, long minKeep);

  int llamaTokenBos(LlamaContext context);

  int llamaTokenEos(LlamaContext context);

  int llamaTokenNl(LlamaContext context);

  void llamaLogSet(BiConsumer<LlamaLogLevel, byte[]> llamaLogCallback);

  long llamaTimeUs();

  LlamaContextParams llamaContextDefaultParams();
  LlamaModelParams llamaModelDefaultParams();

  void llamaSampleTypical(LlamaContext llamaOpaqueContext, LlamaTokenDataArray candidates, float p, int minKeep);

  void llamaSampleTemperature(LlamaContext llamaOpaqueContext, LlamaTokenDataArray candidates, float temp);

  // NEW BATCH STUFF
  LlamaBatch llamaBatchInit(int nTokens, int embd, int nSeqMax);
  void llamaBatchFree(LlamaBatch batch);

  int llamaDecode(LlamaContext ctx, LlamaBatch batch);

}
