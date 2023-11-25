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
