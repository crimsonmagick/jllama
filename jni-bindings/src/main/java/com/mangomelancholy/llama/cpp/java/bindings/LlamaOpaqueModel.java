package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaOpaqueModel {

  public LlamaOpaqueModel(final long modelPointer) {
    this.modelPointer = modelPointer;
  }

  long modelPointer;

  public long getModelPointer() {
    return modelPointer;
  }

  public void setModelPointer(final long modelPointer) {
    this.modelPointer = modelPointer;
  }
}
