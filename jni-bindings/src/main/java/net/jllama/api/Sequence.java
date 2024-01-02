package net.jllama.api;

import static java.util.stream.Collectors.joining;

import java.util.Arrays;
import net.jllama.api.Context.SequenceType;

public class Sequence {

  public static class SequenceId {

    SequenceId(final int[] id) {
      this.id = id;
    }

    int[] id;

    public int[] getId() {
      return id;
    }

    @Override
    public boolean equals(final Object other) {
      if (other == this) {
        return true;
      }
      if (other.getClass() == this.getClass()) {
        final SequenceId otherId = (SequenceId) other;
        return Arrays.equals(otherId.id, this.id);
      }
      return false;
    }

    @Override
    public int hashCode() {
      return Arrays.hashCode(id);
    }

    @Override
    public String toString() {
      return Arrays.stream(id)
          .mapToObj(String::valueOf)
          .collect(joining(";"));
    }
  }

  public static class SequencePiece {

    private final float[] embeddings;
    private final int[] tokens;

    SequencePiece (int[] tokens) {
      this.tokens = Arrays.copyOf(tokens, tokens.length);
      this.embeddings = null;
    }

    SequencePiece (float[] embeddings) {
      this.embeddings = Arrays.copyOf(embeddings, embeddings.length);
      this.tokens= null;
    }
  }
  public Sequence(final int[] sequenceId, final SequenceType sequenceType) {
    this.sequenceId = new SequenceId(sequenceId);
    this.sequenceType = sequenceType;
  }

  SequenceId sequenceId;
  SequenceType sequenceType;

  public SequenceId getSequenceId() {
    return sequenceId;
  }

  public SequenceType getSequenceType() {
    return sequenceType;
  }

  public SequencePiece piece(int[] tokens) {
    if (tokens == null || tokens.length == 0) {
      throw new IllegalArgumentException("A sequence piece cannot be empty.");
    }
    if (sequenceType == SequenceType.EMBEDDING) {
      throw new IllegalArgumentException(
          String.format("Sequence with sequenceId=%s and sequenceType=%s does not support tokens.", sequenceId, sequenceType.name()));
    }
    return new SequencePiece(tokens);
  }

  public SequencePiece piece(float[] embeddings) {
    if (embeddings == null || embeddings.length == 0) {
      throw new IllegalArgumentException("A sequence piece cannot be empty.");
    }
    if (sequenceType == SequenceType.TOKEN) {
      throw new IllegalArgumentException(
          String.format("Sequence with sequenceType=%s does not support embeddings.", sequenceType.name()));
    }
    return new SequencePiece(embeddings);
  }
}
