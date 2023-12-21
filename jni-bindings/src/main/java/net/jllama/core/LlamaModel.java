package net.jllama.core;

import java.io.Closeable;

public class LlamaModel implements Closeable {

  private boolean closed;
  private long modelPointer;

  public LlamaModel(final long modelPointer) {
    this.modelPointer = modelPointer;
    closed = false;
  }

  public static native LlamaModelParams llamaModelDefaultParams();

  private void validateState() {
    if (isClosed()) {
      throw new IllegalStateException("LlamaModel is closed.");
    }
  }

  private native LlamaContext createContextNative(LlamaContextParams llamaContextParams);
  public LlamaContext createContext(LlamaContextParams llamaContextParams) {
    validateState();
    return createContextNative(llamaContextParams);
  }

  private native int tokenizeNative(byte[] text, int[] tokens, int nMaxTokens, boolean addBos);
  public int tokenize(byte[] text, int[] tokens, int nMaxTokens, boolean addBos) {
    validateState();
    return tokenizeNative(text, tokens, nMaxTokens, addBos);
  }

  private native int detokenizeNative(int llamaToken, byte[] buf);

  public int detokenize(int llamaToken, byte[] buf) {
    validateState();
    return detokenizeNative(llamaToken, buf);
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

  private native void closeNative();

  public boolean isClosed() {
    return closed;
  }

  @Override
  public void close() {
    validateState();
    closeNative();
    this.modelPointer = 0;
    closed = true;
  }
}
