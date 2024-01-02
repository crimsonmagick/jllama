package net.jllama.api;

import static net.jllama.api.Context.SequenceType.EMBEDDING;
import static net.jllama.api.Context.SequenceType.TOKEN;

import java.io.Closeable;
import java.util.Arrays;
import net.jllama.api.batch.BatchManager;
import net.jllama.api.batch.BatchSpecifier;
import net.jllama.core.LlamaContext;
import net.jllama.core.LlamaContext.LlamaBatch;
import net.jllama.core.LlamaContextParams;

public class Context {

  public class Batch implements Closeable {

    public static final int DEFAULT_BATCH_SIZE = 1024;
    public static final int DEFAULT_MAX_SEQUENCE_LENGTH = 1;
    private final SequenceType sequenceType;
    private final int batchSize;
    private final LlamaBatch llamaBatch;
    private final int maxSequenceLength;
    private boolean closed;

    Batch(final SequenceType sequenceType, int batchSize, int maxSequenceLength) {
      this.sequenceType = sequenceType;
      final int embeddingsSize = sequenceType == EMBEDDING ? batchSize : 0;
      final int tokensSize = sequenceType == TOKEN ? batchSize : 0;
      this.batchSize = batchSize;
      this.maxSequenceLength = maxSequenceLength;
      llamaBatch = llamaContext.llamaBatchInit(tokensSize, embeddingsSize, maxSequenceLength);
      closed = false;
    }

    @Override
    public void close() {
      if (!closed) {
        llamaBatch.llamaBatchFree();
        closed = true;
      }
    }

    public boolean isClosed() {
      return closed;
    }

    public SequenceType getSequenceType() {
      return sequenceType;
    }

    public int getBatchSize() {
      return batchSize;
    }

    public int getMaxSequenceLength() {
      return maxSequenceLength;
    }

    public LlamaBatch getLlamaBatch() {
      return llamaBatch;
    }

    private void validateState() {
      if (isClosed()) {
        throw new IllegalStateException("Batch has already been closed.");
      }
      if (llamaBatch.isFreed()) {
        throw new IllegalStateException("Underlying LlamaBatch has already been freed.");
      }
    }
  }

  public enum SequenceType {
    EMBEDDING,
    TOKEN
  }

  Context(final LlamaContext llamaContext, final LlamaContextParams llamaContextParams) {
    this.llamaContext = llamaContext;
    this.llamaContextParams = llamaContextParams;
  }

  private Batch embeddingsBatch;
  private Batch tokensBatch;
  private LlamaContext llamaContext;
  private LlamaContextParams llamaContextParams;


  public int getContextSize() {
    return llamaContextParams.getnCtx();
  }

  public LlamaContext getLlamaContext() {
    return llamaContext;
  }

  public BatchSpecifier batch() {
    return new FluentBatch(this);
  }

  public static Sequence sequence(final int[] id, SequenceType type) {
    return new Sequence(Arrays.copyOf(id, id.length), type);
  }

  Batch getEmbeddingsBatch() {
    return embeddingsBatch;
  }

  void setEmbeddingsBatch(final Batch embeddingsBatch) {
    this.embeddingsBatch = embeddingsBatch;
  }

  Batch getTokensBatch() {
    return tokensBatch;
  }

  void setTokensBatch(final Batch tokensBatch) {
    this.tokensBatch = tokensBatch;
  }

}