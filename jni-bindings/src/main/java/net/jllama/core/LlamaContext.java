package net.jllama.core;

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

  private native int llamaDecodeNative(final LlamaBatch batch);
  public int llamaDecode(final LlamaBatch batch) {
    validateState();
    batch.validateState();
    return llamaDecodeNative(batch);
  }

  private native int llamaGetKvCacheUsedCellsNative();

  public int llamaGetKvCacheUsedCells() {
    validateState();
    return llamaGetKvCacheUsedCellsNative();
  }

  private native void llamaKvCacheClearNative();

  public void llamaKvCacheClear() {
    validateState();
    llamaKvCacheClearNative();
  }

  private native void llamaKvCacheSeqRmNative(int seqId, int p0, int p1);
  public void llamaKvCacheSeqRm(int seq_id, int p0, int p1) {
    validateState();
    llamaKvCacheSeqRmNative(seq_id, p0, p1);
  }

  private native void llamaKvCacheSeqCpNative(int seqId, int seqIdDst, int p0, int p1);

  public void llamaKvCacheSeqCp(int seqId, int seqIdDst, int p0, int p1) {
    validateState();
    llamaKvCacheSeqCpNative(seqId, seqIdDst, p0, p1);
  }

  private native void llamaKvCacheSeqKeepNative(int seqId);

  public void llama_kv_cache_seq_keep(int seq_id) {
    validateState();
    llamaKvCacheSeqKeepNative(seq_id);
  }

  private native void llamaKvCacheSeqShiftNative(int seqId, int p0, int p1, int delta);

  public void llamaKvCacheSeqShift(int seq_id, int p0, int p1, int delta) {
    validateState();
    llamaKvCacheSeqShiftNative(seq_id, p0, p1, delta);
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

  public native float[] llamaGetLogitsIthNative(int i);

  public float[] llamaGetLogitsIth(int i) {
    validateState();
    return llamaGetLogitsIthNative(i);
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

  /**
   * Represents a batch of inputs, either tokens or embeddings, to be submitted to llama.cpp to be "decoded" (evaluated).
   * <p>
   * The batch can either be used for the decoding of tokens of embedding values, and thus the  `token` and `embed` arrays are mutually exclusive. If you pass a non-zero value for `embd` to decode(), `token` will be set to null and `embd` will be instantiated. If zero is passed as the `embd` value, `token` will be instantiated and `embd` will be null.
   * <p>
   * Each array is related to the others by way of index. Each array represents a piece of data about a token or embedding value. For example:
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
  public class LlamaBatch implements Closeable {
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

    @Override
    public void close() {
      llamaBatchFree();
    }

    public boolean isFreed() {
      return freed;
    }
  }

}
