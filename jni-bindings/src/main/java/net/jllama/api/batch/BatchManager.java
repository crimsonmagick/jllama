package net.jllama.api.batch;

import net.jllama.api.Context.Batch;

public interface BatchManager {
  BatchConfigurer configure();
  Batch get();
}
