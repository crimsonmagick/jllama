package net.jllama.llama.cpp.java.bindings;

public class LlamaModel {

  long modelPointer;

  public LlamaModel(final long modelPointer) {
    this.modelPointer = modelPointer;
  }

  public long getModelPointer() {
    return modelPointer;
  }

  public void setModelPointer(final long modelPointer) {
    this.modelPointer = modelPointer;
  }
}
