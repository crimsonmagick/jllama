package net.jllama.llama.cpp.java.bindings;

import java.io.Closeable;
import java.util.HashMap;
import net.jllama.llama.cpp.java.bindings.exceptions.LlamaCppException;

public class LlamaContext implements Closeable {

  private long contextPointer;
  private boolean closed;
  private final HashMap<Integer, Sequence> sequences;
  private int contextTokenCount;

  public LlamaContext(final long contextPointer) {
    this.contextPointer = contextPointer;
    closed = false;
    sequences = new HashMap<>();
    contextTokenCount = 0;
  }

  private void validateState() {
    if (isClosed()) {
      throw new IllegalStateException("LlamaModel is closed.");
    }
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
    final int returnValue = evaluateNative(batch);
    if (returnValue < 0) {
      throw new LlamaCppException("LlamaBatch evaluation failed.");
    }
    if (returnValue == 1) {
      throw new LlamaCppException("Could not find a KV slot for the batch (try reducing the size of the batch or increase the context).");
    }

    contextTokenCount += batch.currentTokenCount;
    batch.setCurrentTokenCount(0);
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

  public native float[] getLogitsNative(int batchTokenIndex);

  public float[] getLogits(final Sequence sequence) {
    validateState();
    return getLogitsNative(sequence.getLastLogitPosition());
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

    private native void submitSequenceNative(int[] tokens, int sequenceId, int tokenSequenceIndex);
    public Sequence submitSequence(final int[] tokens) {
      validateState();
      if (currentTokenCount + tokens.length > maxTokenCount) {
        throw new IllegalStateException("LlamaBatch is full.");
      }
      final int sequenceId;
      if (sequences.isEmpty()) {
        sequenceId = 0;
      } else {
        sequenceId = sequences.keySet().stream().max(Integer::compareTo).get() + 1;
      }
      final Sequence sequence = new Sequence();
      sequence.setId(sequenceId);
      sequence.setLength(tokens.length);
      sequence.setLastLogitPosition(tokens.length - 1);
      submitSequenceNative(tokens, sequenceId, 0);
      sequences.put(sequenceId, sequence);
      return sequence;
    }

    public void appendToSequence(final int[] tokens, final Sequence sequence) {
      validateState();
      if (!sequences.containsKey(sequence.getId())) {
        throw new IllegalStateException("Sequence does not exist.");
      }
      if (currentTokenCount + tokens.length > maxTokenCount) {
        throw new IllegalStateException("LlamaBatch is full.");
      }
      submitSequenceNative(tokens, sequence.getId(), sequence.getLength());
      sequence.setLength(sequence.getLength() + tokens.length);
      sequence.setLastLogitPosition(tokens.length - 1);
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

    private native void setCurrentTokenCountNative(int currentTokenCount);
    private void setCurrentTokenCount(final int currentTokenCount) {
      this.currentTokenCount = currentTokenCount;
      setCurrentTokenCountNative(currentTokenCount);
    }

    public int getCurrentTokenCount() {
      return currentTokenCount;
    }
  }

}
