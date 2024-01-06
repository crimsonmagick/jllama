package net.jllama.api;

import java.util.Collections;
import java.util.List;
import net.jllama.api.Context.SequenceType;
import net.jllama.api.util.FloatUtil;
import net.jllama.api.util.IntegerUtil;

public class EmbeddingSequence extends Sequence<Float> {

  public EmbeddingSequence(final int[] sequenceId) {
    super(sequenceId, SequenceType.EMBEDDING);
  }

  @Override
  public Sequence<Float>.SequencePiece piece(final List<Float> embeddings) {
    return piece(embeddings, Collections.singletonList(embeddings.size() - 1));
  }

  @Override
  public Sequence<Float>.SequencePiece piece(final List<Float> embeddings, final List<Integer> logitIndicies) {
    if (embeddings == null || embeddings.isEmpty()) {
      throw new IllegalArgumentException("A sequence piece cannot be empty.");
    }
    if (logitIndicies == null) {
      throw new IllegalArgumentException("logitIndicies must be provided.");
    }
    return new SequencePiece(FloatUtil.toArray(embeddings), IntegerUtil.toArray(logitIndicies));
  }

}