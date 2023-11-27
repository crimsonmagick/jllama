package net.jllama.llama.cpp.java.bindings;

import java.io.Closeable;

public class LlamaContext implements Closeable {

  private long contextPointer;
  private boolean closed;

  public LlamaContext(final long contextPointer) {
    this.contextPointer = contextPointer;
    closed = false;
  }

  private void validateState() {
    if (isClosed()) {
      throw new IllegalStateException("LlamaModel is closed.");
    }
  }

  private native int llamaEvalNative(int[] tokens, int nTokens, int nPast);

  public int llamaEval(int[] tokens, int nTokens, int nPast) {
    validateState();
    return llamaEvalNative(tokens, nTokens, nPast);
  }

  public native void llamaSampleSoftMaxNative(LlamaTokenDataArray candidates);

  public void llamaSampleSoftMax(LlamaTokenDataArray candidates) {
    validateState();
    llamaSampleSoftMaxNative(candidates);
  }

  public native void llamaSampleTopKNative(LlamaTokenDataArray candidates, int k, long minKeep);

  public void llamaSampleTopK(LlamaTokenDataArray candidates, int k, long minKeep) {
    validateState();
    llamaSampleTopKNative(candidates, k, minKeep);
  }

  public native void llamaSampleTopPNative(LlamaTokenDataArray candidates, float p, long minKeep);
  public void llamaSampleTopP(LlamaTokenDataArray candidates, float p, long minKeep) {
    validateState();
    llamaSampleTopPNative(candidates, p, minKeep);
  }

  public native void llamaSampleTailFreeNative(LlamaTokenDataArray candidates, float z, long minKeep);
  public void llamaSampleTailFree(LlamaTokenDataArray candidates, float z, long minKeep) {
    validateState();
    llamaSampleTailFreeNative(candidates, z, minKeep);
  }

  public native void llamaSampleTypicalNative(LlamaTokenDataArray candidates, float p, int minKeep);
  public void llamaSampleTypical(LlamaTokenDataArray candidates, float p, int minKeep) {
    validateState();
    llamaSampleTypicalNative(candidates, p, minKeep);
  }

  public native void llamaSampleTemperatureNative(LlamaTokenDataArray candidates, float temp);
  public void llamaSampleTemperature(LlamaTokenDataArray candidates, float temp) {
    validateState();
    llamaSampleTemperatureNative(candidates, temp);
  }

  public native int llamaTokenBosNative();
  public int llamaTokenBos() {
    validateState();
    return llamaTokenBosNative();
  }

  public native int llamaTokenEosNative();
  public int llamaTokenEos() {
    validateState();
    return llamaTokenEosNative();
  }

  public native int llamaTokenNlNative();
  public int llamaTokenNl() {
    validateState();
    return llamaTokenNlNative();
  }

  public native float[] llamaGetLogitsNative();

  public float[] llamaGetLogits() {
    validateState();
    return llamaGetLogitsNative();
  }

  public native int llamaSampleTokenNative(LlamaTokenDataArray candidates);

  public int llamaSampleToken(LlamaTokenDataArray candidates) {
    validateState();
    return llamaSampleTokenNative(candidates);
  }

  public native int llamaSampleTokenGreedyNative(LlamaTokenDataArray candidates);
  public int llamaSampleTokenGreedy(LlamaTokenDataArray candidates) {
    validateState();
    return llamaSampleTokenGreedyNative(candidates);
  }

  public native void llamaSampleRepetitionPenaltyNative(LlamaTokenDataArray candidates, int[] lastTokens, float penalty);

  public void llamaSampleRepetitionPenalty(LlamaTokenDataArray candidates, int[] lastTokens, float penalty) {
    validateState();
    llamaSampleRepetitionPenaltyNative(candidates, lastTokens, penalty);
  }

  public native void llamaSampleFrequencyAndPresencePenaltiesNative(LlamaTokenDataArray candidates, int[] lastTokens, float alphaFrequency, float alphaPresence);

  public void llamaSampleFrequencyAndPresencePenalties(LlamaTokenDataArray candidates, int[] lastTokens, float alphaFrequency, float alphaPresence) {
    validateState();
    llamaSampleFrequencyAndPresencePenaltiesNative(candidates, lastTokens, alphaFrequency, alphaPresence);
  }

  public static native LlamaContextParams llamaContextDefaultParams();

  private native void closeNative();

  @Override
  public void close() {
    validateState();
    closeNative();
    this.contextPointer = 0;
    closed = true;
  }

  public boolean isClosed() {
    return closed;
  }

}
