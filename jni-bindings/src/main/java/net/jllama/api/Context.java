package net.jllama.api;

import net.jllama.core.LlamaContext;

public class Context {

  private LlamaContext llamaContext;

  public Context(final LlamaContext llamaContext) {
    this.llamaContext = llamaContext;
  }

  public class Batch {

  }

  public LlamaContext getLlamaContext() {
    return llamaContext;
  }
}