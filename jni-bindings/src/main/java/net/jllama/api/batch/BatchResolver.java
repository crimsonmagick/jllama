package net.jllama.api.batch;

import net.jllama.api.Context.SequenceType;

public interface BatchResolver {
  BatchManager type(SequenceType type);
}
