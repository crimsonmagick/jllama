package net.jllama.api;


import static net.jllama.api.Context.SequenceType.EMBEDDING;

import net.jllama.api.Context.SequenceType;
import net.jllama.api.batch.BatchConfigurer;
import net.jllama.api.batch.BatchResolver;
import net.jllama.api.batch.BatchManager;
import net.jllama.api.batch.BatchSpecifier;
import net.jllama.core.LlamaContext.LlamaBatch;

public class FluentBatch implements BatchConfigurer, BatchManager, BatchResolver, BatchSpecifier {

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
      context.setEmbeddingsBatch(buildBatchOnChange(context.getEmbeddingsBatch()));
      return context.getEmbeddingsBatch();
    } else {
      context.setTokensBatch(buildBatchOnChange(context.getTokensBatch()));
      return context.getTokensBatch();
    }
  }

  private Batch buildBatchOnChange(final Batch oldBatch) {
    final Batch batch;
    if (oldBatch == null || hasBatchConfigChanged(oldBatch)) {
      final int isEmbedding = type == EMBEDDING ? 1 : 0;
      final LlamaBatch llamaBatch = context.llamaContext.llamaBatchInit(batchSize, isEmbedding,
          maxSeqIdLength);
      batch = new Batch(type, batchSize, maxSeqIdLength, llamaBatch);
    } else {
      batch = oldBatch;
    }
    if (oldBatch != null && batch != oldBatch) {
      oldBatch.close();
    }
    return batch;
  }

  private boolean hasBatchConfigChanged(final Batch batch) {
    return batch.getBatchSize() != batchSize || batch.getMaxSequenceLength() != maxSeqIdLength;
  }

}
