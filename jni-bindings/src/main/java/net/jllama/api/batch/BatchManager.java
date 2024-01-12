package net.jllama.api.batch;

import net.jllama.api.Batch;

public interface BatchManager {
  BatchConfigurer configure();
  Batch get();
}
