package net.jllama.core;

import java.io.Closeable;
import java.util.HashMap;
import net.jllama.core.exceptions.LlamaCppException;

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

  private native int evaluateNative(LlamaBatchOld batch);

  public void evaluate(final LlamaBatchOld batch) {
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

  private native int llamaDecodeNative(final LlamaBatch batch);
  public int llamaDecode(final LlamaBatch batch) {
    validateState();
    batch.validateState();
    return llamaDecodeNative(batch);
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

  public native float[] getLogitsNative(int batchTokenIndex);

  public float[] getLogits(final Sequence sequence) {
    validateState();
    return getLogitsNative(sequence.getLastLogitPosition());
  }

  public native float[] llamaGetLogitsIthNative(int i);

  public float[] llamaGetLogitsIth(int i) {
    validateState();
    return getLogitsNative(i);
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

  public native void llamaSampleFrequencyAndPresencePenaltiesNative(LlamaTokenDataArray candidates, int[] lastTokens, float alphaFrequency,
      float alphaPresence);

  public void llamaSampleFrequencyAndPresencePenalties(LlamaTokenDataArray candidates, int[] lastTokens, float alphaFrequency, float alphaPresence) {
    validateState();
    llamaSampleFrequencyAndPresencePenaltiesNative(candidates, lastTokens, alphaFrequency, alphaPresence);
  }

  private native LlamaBatchOld llamaBatchInitOldNative(int nTokens, int embd, int nSeqMax);

  public LlamaBatchOld llamaBatchInitOld(int nTokens, int embd, int nSeqMax) {
    validateState();
    return llamaBatchInitOldNative(nTokens, embd, nSeqMax);
  }

  private native LlamaBatch llamaBatchInitNative(final int nTokens, final int embd, final int nSeqMax);

  public LlamaBatch llamaBatchInit(int nTokens, int embd, int nSeqMax) {
    validateState();
    return llamaBatchInitNative(nTokens, embd, nSeqMax);
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

  public class LlamaBatchOld implements Closeable {

    private long batchPointer;
    private boolean closed;
    private int currentTokenCount;
    private final int maxTokenCount;

    private LlamaBatchOld(final long batchPointer, final int maxTokenCount) {
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

    private native void submitSequenceOldNative(int[] tokens, int sequenceId, int tokenSequenceIndex);

//    public native void submitSequencePieceNative(int[] token, float[] embd, int[] pos, int[][] seq_id, boolean[] logits);
//
//    public void submitSequencePiece(int[] token, float[] embd, int[] pos, int[][] seq_id, boolean[] logits) {
//      validateState();
//    }

    public Sequence submitSequenceOld(final int[] tokens) {
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
      submitSequenceOldNative(tokens, sequenceId, 0);
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
      submitSequenceOldNative(tokens, sequence.getId(), sequence.getLength());
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

  /**
   * Represents a batch of inputs, either tokens or embeddings, to be submitted to llama.cpp to be "decoded" (evaluated).
   *
   * The batch can either be used for the decoding of tokens of embedding values, and thus the  `token` and `embed` arrays are mutually exclusive. If you pass a non-zero value for `embd` to decode(), `token` will be set to null and `embd` will be instantiated. If zero is passed as the `embd` value, `token` will be instantiated and `embd` will be null.
   *
   * Each array is related to the others by way of index. Each array represents a piece of data about a token or embedding value. For example:
   *
   * {@code
   * int tokens = getTokens(); // assume the program has already provided an array of tokens
   * LlamaBatch batch = llamaContext.llamaBatchInit(40, 0, 1); // sets array sizes for token, pos, nSeqId, seqId, and logits to 40
   * batch.tokens[0] = tokens[0]; // set the first token, token 0
   * batch.seqId[0] = 23; // Token 0 is arbitrarily assigned a seqId of 23. A context can have one or more sequences, which have been set to be a maxmium of 1 in length
   * batch.nSeqId[0] = 1; // we set the max length of the seqId to 1, so this can't be any other value
   * batch.pos[0] = 12; // This means 11 tokens have already been evaluated for this sequence, sequence 23
   * batch.logits[0] = 1; // logits can be decoded for each character in the batch. Setting the value to 1 for a token enables decoding the logits, setting to 0 disables. In a typical chat application, you'd only want to set the last token in the batch to 1
   * llamaContext.decode(batch); // submit the token to the LlamaContext to be decoded/evaluated along with all other tokens currently in its context
   * // logic for getting logits goes here}
   *
   * {@link LlamaBatch#nTokens nTokens} represents the number of tokens or embedding values you want decoded from this batch. All
   *
   * @see LlamaContext#llamaDecode(LlamaBatch)
   */
  public class LlamaBatch {

    private final long batchPointer;
    private boolean freed;
    public int nTokens;
    public int[] token;
    public float[] embd;
    public int[] pos;
    public int[] nSeqId;
    public int[][] seqId;
    public byte[] logits;

    public LlamaBatch(long batchPointer, int nTokens, int[] token, float[] embd, int[] pos, int[] nSeqId, int[][] seqId, byte[] logits) {
      this.batchPointer = batchPointer;
      this.freed = false;
      this.nTokens = nTokens;
      this.token = token;
      this.embd = embd;
      this.pos = pos;
      this.nSeqId = nSeqId;
      this.seqId = seqId;
      this.logits = logits;
    }

    private native void llamaBatchFreeNative();

    private void validateState() {
      if (isFreed()) {
        throw new IllegalStateException("LlamaBatch has already been freed.");
      }
    }

    public void llamaBatchFree() {
      validateState();
      llamaBatchFreeNative();
      freed = true;
    }

    public boolean isFreed() {
      return freed;
    }
  }

}
