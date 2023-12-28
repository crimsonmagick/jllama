package net.jllama.api;

import net.jllama.api.Context.SequenceType;

public class Sequence {

  public Sequence(final int[] sequenceId, final SequenceType sequenceType) {
    this.sequenceId = sequenceId;
    this.sequenceType = sequenceType;
  }

  int[] sequenceId;
  SequenceType sequenceType;

  public int[] getSequenceId() {
    return sequenceId;
  }

  public SequenceType getSequenceType() {
    return sequenceType;
  }
}
