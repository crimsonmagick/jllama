package net.jllama.api;

import net.jllama.core.LlamaContext;
import net.jllama.core.LlamaContextParams;
import net.jllama.core.LlamaModel;

public class Model {

  public Model(final LlamaModel llamaModel) {
    this.llamaModel = llamaModel;
  }

  private boolean closed;
  private final LlamaModel llamaModel;

  public ContextConfigurer newContext() {
    return new ContextConfigurer(this);
  }

  Context createContext(final LlamaContextParams llamaContextParams) {
    final LlamaContext llamaContext = llamaModel.createContext(llamaContextParams);
    return new Context(llamaContext);
  }

  public boolean isClosed() {
    return closed;
  }

  public LlamaModel getLlamaModel() {
    return llamaModel;
  }

}
