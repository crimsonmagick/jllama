package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaOpaqueContext {

  long contextPointer;

  public LlamaOpaqueContext(final long contextPointer) {
    this.contextPointer = contextPointer;
  }

  public long getContextPointer() {
    return contextPointer;
  }

  public void setContextPointer(final long contextPointer) {
    this.contextPointer = contextPointer;
  }
}
