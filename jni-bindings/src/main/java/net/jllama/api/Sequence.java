package net.jllama.api;

import static java.util.stream.Collectors.joining;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import net.jllama.api.Context.SequenceType;
import net.jllama.api.util.FloatUtil;
import net.jllama.api.util.IntegerUtil;

public abstract class Sequence<T extends Number> {

  public static class SequenceId {

    // FIXME this especially needs a refactor. A SequenceId is really an int, and that sequence in turn can have sub-sequences; a sub-sequence is a "draft" sequence used for speculative evaluation/decoding where the first int of the "id" is the "parent" id, and the subsequent ids distinguish it as a "draft." The decode engine of llama.cpp then uses submitted draft tokens to help "guide" the next decoding of logits. Not sure about its use with embeddings.
    SequenceId(final int[] id) {
      this.id = id;
    }

    int[] id;

    public List<Integer> getId() {
      return IntegerUtil.toList(id);
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

    int length;
    float[] embeddings;
    LinkedHashMap<Integer, Integer> logitsIndiciesMap;
    int[] tokens;
    int[] logitIndicies;

    protected SequencePiece(final int[] tokens, final int[] logitIndicies) {
      this.tokens = Arrays.copyOf(tokens, tokens.length);
      length = tokens.length;
      this.logitIndicies = Arrays.copyOf(logitIndicies, logitIndicies.length);
      this.embeddings = null;
    }

    protected SequencePiece(final float[] embeddings, final int[] logitIndicies) {
      this.embeddings = Arrays.copyOf(embeddings, embeddings.length);
      length = embeddings.length;
      this.logitIndicies = Arrays.copyOf(logitIndicies, logitIndicies.length);
      this.tokens = null;
    }

    public List<Float> getEmbeddings() {
      return FloatUtil.toList(embeddings);
    }

    public List<Integer> getTokens() {
      return IntegerUtil.toList(tokens);
    }

    public List<Integer> getLogitIndicies() {
      return IntegerUtil.toList(logitIndicies);
    }

    public Sequence<T> getSequence() {
      return Sequence.this;
    }

    public int getLength() {
      return length;
    }

    LinkedHashMap<Integer, Integer> getLogitsIndiciesMap() {
      return logitsIndiciesMap;
    }

    void setLogitsIndiciesMap(final LinkedHashMap<Integer, Integer> relativeToAbsoluteLogits) {
      this.logitsIndiciesMap = relativeToAbsoluteLogits;
    }
  }

  Sequence(final int[] sequenceId, final SequenceType sequenceType) {
    this.sequenceId = new SequenceId(sequenceId);
    this.sequenceType = sequenceType;
    length = 0;
    this.lastLogitIndiciesMap = Collections.emptyMap();
  }

  Map<Integer, Integer> lastLogitIndiciesMap;
  SequenceId sequenceId;
  SequenceType sequenceType;
  int length;

  public SequenceId getSequenceId() {
    return sequenceId;
  }

  public SequenceType getSequenceType() {
    return sequenceType;
  }

  public abstract SequencePiece piece(List<T> tokensOrEmbeddings);


  public abstract SequencePiece piece(List<T> tokensOrEmbeddings, List<Integer> logitIndicies);

  public int getLength() {
    return length;
  }

  void setLength(final int length) {
    this.length = length;
  }

  public List<Integer> getLastLogitIndicies() {
    return new ArrayList<>(lastLogitIndiciesMap.values());
  }

  public Map<Integer, Integer> getLastLogitIndiciesMap() {
    return lastLogitIndiciesMap;
  }

  // require caller to preserve insertion order
  void setLastLogitIndiciesMap(final LinkedHashMap<Integer, Integer> logitIndicies) {
    lastLogitIndiciesMap = logitIndicies;
  }

  public static SequenceId sequenceId(final int id) {
    return new SequenceId(new int[]{id});
  }

  public static Sequence<Integer> tokenSequence(final int id) {
    return tokenSequence(Collections.singletonList(id));
  }

  public static Sequence<Integer> tokenSequence(final List<Integer> id) {
    return new TokenSequence(IntegerUtil.toArray(id));
  }

  public static Sequence<Float> embeddingSequence(final int id) {
    return embeddingSequence(Collections.singletonList(id));
  }

  public static Sequence<Float> embeddingSequence(final List<Integer> id) {
    return new EmbeddingSequence(IntegerUtil.toArray(id));
  }
}
