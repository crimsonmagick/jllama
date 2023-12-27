package net.jllama.api;

import java.io.Closeable;
import net.jllama.core.LlamaContext;
import net.jllama.core.LlamaContextParams;
import net.jllama.core.LlamaModel;

public class Model implements Closeable {

  public Model(final LlamaModel llamaModel) {
    this.llamaModel = llamaModel;
    this.tokens = new Tokens(llamaModel);
  }

  private final LlamaModel llamaModel;
  private final Tokens tokens;
  private boolean closed;

  @Override
  public void close() {
    if (!llamaModel.isClosed()) {
      llamaModel.close();
    }
    closed = true;
  }
  Context createContext(final LlamaContextParams llamaContextParams) {
    final LlamaContext llamaContext = llamaModel.createContext(llamaContextParams);
    return new Context(llamaContext);
  }

  public LlamaModel getLlamaModel() {
    return llamaModel;
  }

  public ContextConfigurer newContext() {
    return new ContextConfigurer(this);
  }

  public boolean isClosed() {
    return closed;
  }

  public Tokens tokens() {
    return tokens;
  }

  private void validateState() {
    if (isClosed()) {
      throw new IllegalStateException("Model has already been freed.");
    }
    if (llamaModel.isClosed()) {
      throw new IllegalStateException("Underlying LlamaModel has already been freed.");
    }
  }

}
