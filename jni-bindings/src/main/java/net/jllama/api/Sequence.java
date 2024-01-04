package net.jllama.api;

import static java.util.stream.Collectors.joining;

import java.util.Arrays;
import java.util.Collections;
import java.util.Map;
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

  public class SequencePiece {

    private final int length;
    private final float[] embeddings;
    private Map<Integer, Integer> logitsIndiciesMap;
    private final int[] tokens;
    private final int[] logitIndicies;

    private SequencePiece(final int[] tokens, final int[] logitIndicies) {
      this.tokens = Arrays.copyOf(tokens, tokens.length);
      length = tokens.length;
      this.logitIndicies = Arrays.copyOf(logitIndicies, logitIndicies.length);
      this.embeddings = null;
    }

    private SequencePiece(final float[] embeddings, final int[] logitIndicies) {
      this.embeddings = Arrays.copyOf(embeddings, embeddings.length);
      length = embeddings.length;
      this.logitIndicies = Arrays.copyOf(logitIndicies, logitIndicies.length);
      this.tokens = null;
    }

    public float[] getEmbeddings() {
      return embeddings;
    }

    public int[] getTokens() {
      return tokens;
    }

    public int[] getLogitIndicies() {
      return logitIndicies;
    }

    public Sequence getSequence() {
      return Sequence.this;
    }

    public int getLength() {
      return length;
    }

    public Map<Integer, Integer> getLogitsIndiciesMap() {
      return logitsIndiciesMap;
    }

    public void setLogitsIndiciesMap(final Map<Integer, Integer> relativeToAbsoluteLogits) {
      this.logitsIndiciesMap = relativeToAbsoluteLogits;
    }
  }

  public Sequence(final int[] sequenceId, final SequenceType sequenceType) {
    this.sequenceId = new SequenceId(sequenceId);
    this.sequenceType = sequenceType;
    length = 0;
    this.lastLogitIndiciesMap = Collections.emptyMap();
  }

  private Map<Integer, Integer> lastLogitIndiciesMap;
  private SequenceId sequenceId;
  private SequenceType sequenceType;
  private int length;

  public SequenceId getSequenceId() {
    return sequenceId;
  }

  public SequenceType getSequenceType() {
    return sequenceType;
  }

  public SequencePiece piece(int[] tokens, int[] logitIndicies) {
    if (tokens == null || tokens.length == 0) {
      throw new IllegalArgumentException("A sequence piece cannot be empty.");
    }
    if (logitIndicies == null) {
      throw new IllegalArgumentException("logitIndicies must be provided.");
    }
    if (sequenceType == SequenceType.EMBEDDING) {
      throw new IllegalArgumentException(
          String.format("Sequence with sequenceId=%s and sequenceType=%s does not support tokens.", sequenceId, sequenceType.name()));
    }
    return new SequencePiece(tokens, logitIndicies);
  }

  public SequencePiece piece(float[] embeddings, int[] logitIndicies) {
    if (embeddings == null || embeddings.length == 0) {
      throw new IllegalArgumentException("A sequence piece cannot be empty.");
    }
    if (logitIndicies == null) {
      throw new IllegalArgumentException("logitIndicies must be provided.");
    }
    if (sequenceType == SequenceType.TOKEN) {
      throw new IllegalArgumentException(
          String.format("Sequence with sequenceType=%s does not support embeddings.", sequenceType.name()));
    }
    return new SequencePiece(embeddings, logitIndicies);
  }

  public int getLength() {
    return length;
  }

  void setLength(final int length) {
    this.length = length;
  }

  public int[] getLastLogitIndicies() {
    return lastLogitIndiciesMap.values()
        .stream()
        .mapToInt(Integer::valueOf)
        .toArray();
  }

  public Map<Integer, Integer> getLastLogitIndiciesMap() {
    return lastLogitIndiciesMap;
  }

  void setLastLogitIndiciesMap(final Map<Integer, Integer> logitIndicies) {
    lastLogitIndiciesMap = logitIndicies;
  }

  public static Sequence sequence(final int[] id, final SequenceType type) {
    return new Sequence(Arrays.copyOf(id, id.length), type);
  }
}
