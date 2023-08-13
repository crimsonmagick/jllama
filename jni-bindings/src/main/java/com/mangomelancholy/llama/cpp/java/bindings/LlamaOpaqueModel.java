package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaOpaqueModel {

  long modelPointer;

  public LlamaOpaqueModel(final long modelPointer) {
    this.modelPointer = modelPointer;
  }

  public long getModelPointer() {
    return modelPointer;
  }

  public void setModelPointer(final long modelPointer) {
    this.modelPointer = modelPointer;
  }
}
