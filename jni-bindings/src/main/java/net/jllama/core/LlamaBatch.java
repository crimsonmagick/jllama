package net.jllama.core;

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
public class LlamaBatch implements AutoCloseable {
  private final long batchPointer;
  private boolean freed;
  public int nTokens;
  public int[] token;
  public float[] embd;
  public int[] pos;
  public int[] nSeqId;
  public int[][] seqId;
  public byte[] logits;

  public LlamaBatch(long batchPointer, int nTokens, int[] token, float[] embd, int[] pos,
      int[] nSeqId, int[][] seqId, byte[] logits) {
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

  void validateState() {
    if (isFreed()) {
      throw new IllegalStateException("LlamaBatch has already been closed/freed.");
    }
  }

  @Override
  public void close() {
    validateState();
    llamaBatchFreeNative();
    freed = true;
  }

  public boolean isFreed() {
    return freed;
  }
}