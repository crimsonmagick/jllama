package net.jllama.core;

public class LlamaContext implements AutoCloseable {

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

  public void llamaKvCacheSeqKeep(int seq_id) {
    validateState();
    llamaKvCacheSeqKeepNative(seq_id);
  }

  private native void llamaKvCacheSeqShiftNative(int seqId, int p0, int p1, int delta);

  public void llamaKvCacheSeqShift(int seq_id, int p0, int p1, int delta) {
    validateState();
    llamaKvCacheSeqShiftNative(seq_id, p0, p1, delta);
  }

  private native void llamaSampleRepetitionPenaltiesNative(LlamaTokenDataArray candidates,
      int[] lastTokens, long penaltyLastN, float penaltyRepeat, float penaltyFreq,
      float penaltyPresent);

  public void llamaSampleRepetitionPenalties(LlamaTokenDataArray candidates, int[] lastTokens,
      long penaltyLastN, float penaltyRepeat, float penaltyFreq, float penaltyPresent) {
    validateState();
    llamaSampleRepetitionPenaltiesNative(candidates, lastTokens, penaltyLastN, penaltyRepeat, penaltyFreq, penaltyPresent);
  }

  private native void llamaSampleSoftmaxNative(LlamaTokenDataArray candidates);

  public void llamaSampleSoftmax(LlamaTokenDataArray candidates) {
    validateState();
    llamaSampleSoftmaxNative(candidates);
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

  private native void llamaSampleMinPNative(LlamaTokenDataArray candidates, float p, long minKeep);

  public void llamaSampleMinP(LlamaTokenDataArray candidates, float p, long MinKeep) {
    validateState();
    llamaSampleMinPNative(candidates, p, MinKeep);
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

  public native void llamaSampleTempNative(LlamaTokenDataArray candidates, float temp);

  public void llamaSampleTemp(LlamaTokenDataArray candidates, float temp) {
    validateState();
    llamaSampleTempNative(candidates, temp);
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



}
