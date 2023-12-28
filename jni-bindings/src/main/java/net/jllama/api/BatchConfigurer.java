package net.jllama.api;


import net.jllama.api.Context.Batch;
import net.jllama.api.Context.SequenceType;

public class BatchConfigurer {

  final Context context;

  BatchConfigurer(final Context context) {
    this.context = context;
  }

  public class BatchBuilder {
    private int maxSequenceLength;
    private int batchSize;
    private SequenceType sequenceType;

    public BatchBuilder maxSequenceLength(final int maxSequenceLength) {
      this.maxSequenceLength = maxSequenceLength;
      return this;
    }

    public BatchBuilder batchSize(final int batchSize) {
      this.batchSize = batchSize;
      return this;
    }

    public BatchBuilder batchType(final SequenceType sequenceType) {
      this.sequenceType = sequenceType;
      return this;
    }

    public Batch build() {
      if (sequenceType == null) {
        throw new IllegalStateException("Batch type must be specified.");
      }
      if (batchSize <= 0) {
        throw new IllegalStateException("Batch size must be greater than 0.");
      }
      if (maxSequenceLength <= 0) {
        throw new IllegalStateException("Max sequence length must be greater than 0.");
      }
      return context.new Batch(sequenceType, batchSize, maxSequenceLength);
    }

  }

  public BatchBuilder with() {
    return new BatchBuilder();
  }

}
