package net.jllama.api.batch;

public interface BatchManager {
  BatchConfigurer configure();
  Batch get();
}
