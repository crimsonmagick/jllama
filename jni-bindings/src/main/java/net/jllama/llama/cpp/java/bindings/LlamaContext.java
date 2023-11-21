package net.jllama.llama.cpp.java.bindings;

public class LlamaContext {

  long contextPointer;

  public LlamaContext(final long contextPointer) {
    this.contextPointer = contextPointer;
  }

  public long getContextPointer() {
    return contextPointer;
  }

  public void setContextPointer(final long contextPointer) {
    this.contextPointer = contextPointer;
  }
}
