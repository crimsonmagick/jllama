package net.jllama.api.batch;

import net.jllama.api.Batch;

public interface BatchConfigurer {

  BatchConfigurer batchSize(int batchSize);
  Batch get();
  BatchConfigurer update();
  BatchConfigurer maxSequenceLength(int maxSequenceLength);

}
