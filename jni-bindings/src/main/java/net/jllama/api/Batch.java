package net.jllama.api;

import static net.jllama.api.Context.SequenceType.TOKEN;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import net.jllama.api.Context.SequenceType;
import net.jllama.api.Sequence.SequenceId;
import net.jllama.api.Sequence.SequencePiece;
import net.jllama.core.LlamaBatch;

public class Batch implements AutoCloseable {

  public static final int DEFAULT_BATCH_SIZE = 1024;
  public static final int DEFAULT_MAX_SEQUENCE_LENGTH = 1;
  private final SequenceType sequenceType;
  private final int batchSize;
  private final LlamaBatch llamaBatch;
  private final int maxSequenceLength;
  private boolean closed;
  private final Map<SequenceId, SequencePiece> stagedSequences;

  public Batch(final SequenceType sequenceType, int batchSize, int maxSequenceLength, final LlamaBatch llamaBatch) {
    this.sequenceType = sequenceType;
    this.batchSize = batchSize;
    this.maxSequenceLength = maxSequenceLength;
    this.llamaBatch = llamaBatch;
    stagedSequences = new HashMap<>();
    closed = false;
  }

  @Override
  public void close() {
    if (closed) {
      throw new IllegalStateException("Batch has already been closed.");
    }
    llamaBatch.close();
    closed = true;
  }

  public boolean isClosed() {
    return closed;
  }

  public SequenceType getSequenceType() {
    return sequenceType;
  }

  public int getBatchSize() {
    return batchSize;
  }

  public int getMaxSequenceLength() {
    return maxSequenceLength;
  }

  public List<SequencePiece> getStagedSequences() {
    return new ArrayList<>(stagedSequences.values());
  }

  public void clear() {
    stagedSequences.clear();
  }

  public LlamaBatch getLlamaBatch() {
    return llamaBatch;
  }

  // TODO shouldn't we just do this at stage time?????
  public LlamaBatch prepare() {
    validateState();
    int batchPos = 0;
    for (final Entry<SequenceId, SequencePiece> entry : stagedSequences.entrySet()) {
      final SequenceId sequenceId = entry.getKey();
      final Sequence<?>.SequencePiece piece = entry.getValue();
      final Sequence<?> sequence = entry.getValue().getSequence();
      final int sequenceBatchStartPos = batchPos;

      int seqPos = sequence.getLength();

      for (int i = 0; i < piece.length; i++) {
        if (sequenceType == TOKEN) {
          llamaBatch.token[batchPos] = piece.tokens[i];
        } else {
          llamaBatch.embd[batchPos] = piece.embeddings[i];
        }
        llamaBatch.pos[batchPos] = seqPos;
        llamaBatch.seqId[batchPos] = sequenceId.id;
        llamaBatch.logits[batchPos] = 0;
        llamaBatch.nSeqId[batchPos] = sequenceId.id.length;
        batchPos += 1;
        seqPos += 1;
      }
      final LinkedHashMap<Integer, Integer> relativeToAbsolute = new LinkedHashMap<>(piece.logitIndicies.length);
      for (int i : piece.logitIndicies) {
        final int absolutePos = sequenceBatchStartPos + i;
        llamaBatch.logits[absolutePos] = 1;
        relativeToAbsolute.put(i, absolutePos);
      }
      piece.setLogitsIndiciesMap(relativeToAbsolute);
    }
    llamaBatch.nTokens = batchPos;
    return llamaBatch;
  }

  public void stage(final SequencePiece sequencePiece) {
    validatePiece(sequencePiece);
    stagedSequences.put(sequencePiece.getSequence().getSequenceId(), sequencePiece);
  }

  private void validatePiece(final SequencePiece sequencePiece) {
    if (stagedSequences.containsKey(sequencePiece.getSequence().getSequenceId())) {
      throw new IllegalArgumentException(
          String.format("A piece with sequenceId=%s has already been staged. Please clear the batch.", sequencePiece.getSequence().getSequenceId()));
    }
    final int currentStagedLength = this.stagedSequences.values()
        .stream()
        .mapToInt(SequencePiece::getLength)
        .sum();
    final int newStagedLength = currentStagedLength + sequencePiece.getLength();
    if (newStagedLength > batchSize) {
      throw new IllegalArgumentException("Cannot stage sequencePiece, new totalStagedLength=%s would exceed batchSize=%s");
    }
  }

  private void validateState() {
    if (isClosed()) {
      throw new IllegalStateException("Batch has already been closed.");
    }
    if (llamaBatch.isFreed()) {
      throw new IllegalStateException("Underlying LlamaBatch has already been freed.");
    }
  }
}