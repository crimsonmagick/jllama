package net.jllama.api;

import java.util.Collections;
import java.util.List;
import net.jllama.api.Context.SequenceType;
import net.jllama.api.util.IntegerUtil;

public class TokenSequence extends Sequence<Integer> {

  public TokenSequence(final int[] sequenceId) {
    super(sequenceId, SequenceType.TOKEN);
  }

  @Override
  public Sequence<Integer>.SequencePiece piece(final List<Integer> tokens) {
    return piece(tokens, Collections.singletonList(tokens.size() - 1));
  }

  @Override
  public Sequence<Integer>.SequencePiece piece(final List<Integer> tokens, final List<Integer> logitIndicies) {
    if (tokens == null || tokens.isEmpty()) {
      throw new IllegalArgumentException("A sequence piece cannot be empty.");
    }
    if (logitIndicies == null) {
      throw new IllegalArgumentException("logitIndicies must be provided.");
    }
    return new SequencePiece(IntegerUtil.toArray(tokens), IntegerUtil.toArray(logitIndicies));
  }

}