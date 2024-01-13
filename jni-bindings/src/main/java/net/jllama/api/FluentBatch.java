package net.jllama.api;


import static net.jllama.api.Context.SequenceType.EMBEDDING;

import net.jllama.api.Context.SequenceType;
import net.jllama.api.batch.BatchConfigurer;
import net.jllama.api.batch.BatchManager;
import net.jllama.api.batch.BatchSpecifier;
import net.jllama.core.LlamaBatch;

public class FluentBatch implements BatchConfigurer, BatchManager, BatchSpecifier {

  final Context context;
  private SequenceType type;
  private int batchSize;
  private int maxSeqIdLength;

  FluentBatch(final Context context) {
    this.context = context;
    batchSize = Batch.DEFAULT_BATCH_SIZE;
    maxSeqIdLength = Batch.DEFAULT_MAX_SEQUENCE_LENGTH;
  }

  @Override
  public BatchManager type(final SequenceType type) {
    this.type = type;
    return this;
  }

  @Override
  public BatchConfigurer configure() {
    return this;
  }

  @Override
  public BatchConfigurer batchSize(final int batchSize) {
    if (batchSize <= 0) {
      throw new IllegalStateException("Batch size must be greater than 0.");
    }
    this.batchSize = batchSize;
    return this;
  }

  @Override
  public BatchConfigurer maxSequenceLength(final int maxSequenceLength) {
    if (maxSequenceLength <= 0) {
      throw new IllegalStateException("Max sequence length must be greater than 0.");
    }
    this.maxSeqIdLength = maxSequenceLength;
    return this;
  }

  @Override
  public Batch get() {
    if (type == SequenceType.EMBEDDING) {
      if (context.getEmbeddingsBatch() == null) {
        context.setEmbeddingsBatch(buildBatch());
      }
      return context.getEmbeddingsBatch();
    } else {
      if (context.getTokensBatch() == null) {
        context.setTokensBatch(buildBatch());
      }
      return context.getTokensBatch();
    }
  }

  @Override
  public BatchConfigurer update() {
    if (type == SequenceType.EMBEDDING) {
      context.setEmbeddingsBatch(buildBatch());
    } else {
      context.setTokensBatch(buildBatch());
    }
    return this;
  }

  private Batch buildBatch() {
    final int isEmbedding = type == EMBEDDING ? 1 : 0;
    final LlamaBatch llamaBatch = context.llamaContext.llamaBatchInit(batchSize, isEmbedding,
        maxSeqIdLength);
    return new Batch(type, batchSize, maxSeqIdLength, llamaBatch);
  }

}
