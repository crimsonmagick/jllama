package net.jllama.api;

import static net.jllama.api.Context.SequenceType.EMBEDDING;
import static net.jllama.api.Context.SequenceType.TOKEN;

import net.jllama.core.LlamaContext;
import net.jllama.core.LlamaContext.LlamaBatch;
import net.jllama.core.LlamaContextParams;

public class Context {

  private LlamaContext llamaContext;
  private LlamaContextParams llamaContextParams;

  Context(final LlamaContext llamaContext, final LlamaContextParams llamaContextParams) {
    this.llamaContext = llamaContext;
    this.llamaContextParams = llamaContextParams;
  }

  public int getContextSize() {
    return llamaContextParams.getnCtx();
  }

  public LlamaContext getLlamaContext() {
    return llamaContext;
  }
  public enum SequenceType {
    EMBEDDING,
    TOKEN
  }

  public class Batch {
    private final SequenceType sequenceType;
    private final LlamaBatch llamaBatch;

    Batch(final SequenceType sequenceType, int batchSize, int maxSequenceLength) {
      this.sequenceType = sequenceType;
      final int embeddingsSize = sequenceType == EMBEDDING ? batchSize : 0;
      final int tokensSize = sequenceType == TOKEN ? batchSize : 0;
      llamaBatch = llamaContext.llamaBatchInit(embeddingsSize, tokensSize, maxSequenceLength);
    }

    public SequenceType getSequenceType() {
      return sequenceType;
    }
  }
}