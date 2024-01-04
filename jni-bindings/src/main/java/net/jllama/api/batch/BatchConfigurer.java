package net.jllama.api.batch;

public interface BatchConfigurer {

  BatchConfigurer batchSize(int batchSize);
  Batch get();
  BatchConfigurer maxSequenceLength(int maxSequenceLength);

}
