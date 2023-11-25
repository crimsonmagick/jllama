package net.jllama.llama.cpp.java.bindings;

import java.io.Closeable;
import java.io.IOException;

public class LlamaModel implements Closeable {

  boolean closed;
  long modelPointer;

  public LlamaModel(final long modelPointer) {
    this.modelPointer = modelPointer;
    closed = false;
  }

  public LlamaContext createContext(LlamaContextParams llamaContextParams) {
    if (isClosed()) {
      throw new IllegalStateException("LlamaModel is closed.");
    }
    return newContext(llamaContextParams);
  }
  private native LlamaContext newContext(LlamaContextParams llamaContextParams);

  private native void close(long modelPointer);

  public long getModelPointer() {
    return modelPointer;
  }

  public void setModelPointer(final long modelPointer) {
    this.modelPointer = modelPointer;
  }

  public boolean isClosed() {
    return closed;
  }

  @Override
  public void close() throws IOException {
    close(modelPointer);
    this.modelPointer = 0;
    closed = true;
  }
}
