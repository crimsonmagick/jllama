package net.jllama.llama.cpp.java.bindings;

import java.io.Closeable;
import java.util.Set;

public class LlamaContext implements Closeable {

  private long contextPointer;
  private boolean closed;

  private Set<Integer> sequences;

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

  private native int evaluateNative(LlamaBatch batch);
  public void evaluate(final LlamaBatch batch) {
    validateState();
    if (batch.isClosed()) {
      throw new IllegalStateException("LlamaBatch is closed.");
    }
    if (batch.getCurrentTokenCount() == 0) {
      throw new IllegalStateException("LlamaBatch is empty.");
    }
    evaluateNative(batch);
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

  private native LlamaBatch createBatchNative(int maxTokenCount, int embeddingVectorSize, int sequenceIdLength);

  public LlamaBatch createBatch(int maxTokenCount) {
    validateState();
    return createBatchNative(maxTokenCount,0, 1);
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

  public class LlamaBatch implements Closeable {
    private long batchPointer;
    private boolean closed;
    private int currentTokenCount;
    private final int maxTokenCount;

    private LlamaBatch(final long batchPointer, final int maxTokenCount) {
      this.batchPointer = batchPointer;
      this.maxTokenCount = maxTokenCount;
      this.currentTokenCount = 0;
      closed = false;
    }

    private void validateState() {
      if (isClosed()) {
        throw new IllegalStateException("LlamaBatch is closed.");
      }
    }

    private native void submitSequenceNative(int[] tokens, int sequenceId);
    public int submitSequence(final int[] tokens) {
      validateState();
      if (currentTokenCount + tokens.length > maxTokenCount) {
        throw new IllegalStateException("LlamaBatch is full.");
      }
      final int sequenceId;
      if (sequences.isEmpty()) {
        sequenceId = 0;
      } else {
        sequenceId = sequences.stream().max(Integer::compareTo).get() + 1;
      }
      sequences.add(sequenceId);
      submitSequenceNative(tokens, sequenceId);
      currentTokenCount += tokens.length;
      return sequenceId;
    }

    public void appendToSequence(final int[] tokens, final int sequenceId) {
      validateState();
      if (!sequences.contains(sequenceId)) {
        throw new IllegalStateException("Sequence does not exist.");
      }
      if (currentTokenCount + tokens.length > maxTokenCount) {
        throw new IllegalStateException("LlamaBatch is full.");
      }
      submitSequenceNative(tokens, sequenceId);
      currentTokenCount += tokens.length;
    }

    private native void closeNative();

    @Override
    public void close() {
      validateState();
      closeNative();
      this.batchPointer = 0;
      closed = true;
    }

    public boolean isClosed() {
      return closed;
    }

    public int getMaxTokenCount() {
      return maxTokenCount;
    }

    public int getCurrentTokenCount() {
      return currentTokenCount;
    }
  }

}
