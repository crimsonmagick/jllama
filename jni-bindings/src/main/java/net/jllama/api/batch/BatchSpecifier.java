package net.jllama.api.batch;

import net.jllama.api.Context.Batch;
import net.jllama.api.Context.SequenceType;

public interface BatchSpecifier {
  BatchManager type(SequenceType type);
}
