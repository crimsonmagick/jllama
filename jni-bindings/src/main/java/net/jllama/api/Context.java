package net.jllama.api;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import net.jllama.api.Sequence.SequenceId;
import net.jllama.api.Sequence.SequencePiece;
import net.jllama.api.batch.BatchSpecifier;
import net.jllama.api.exceptions.LlamaApiException;
import net.jllama.api.util.FloatUtil;
import net.jllama.core.LlamaContext;
import net.jllama.core.LlamaContextParams;

public class Context implements AutoCloseable {

  public enum SequenceType {
    EMBEDDING,
    TOKEN
  }
  private final LlamaContextParams llamaContextParams;
  private final Map<SequenceId, Sequence<?>> sequences;
  LlamaContext llamaContext;
  private boolean closed;
  private Batch embeddingsBatch;
  private Batch tokensBatch;

  Context(final LlamaContext llamaContext, final LlamaContextParams llamaContextParams) {
    this.llamaContext = llamaContext;
    this.llamaContextParams = llamaContextParams;
    this.sequences = new HashMap<>();
    this.closed = false;
  }

  public BatchSpecifier batch() {
    return new FluentBatch(this);
  }

  public void clearSequences() {
    validateState();
    sequences.values().forEach(sequence -> sequence.setLength(0));
    sequences.clear();
    llamaContext.llamaKvCacheClear();
  }

  @Override
  public void close() {
    validateState();
    llamaContext.close();
    closed = true;
  }

  public Context evaluate(final Batch batch) {
    validateStaged(batch);
    final int ret = llamaContext.llamaDecode(batch.prepare());
    if (ret != 0) {
      throw new LlamaApiException("Decode failed with returnValue=" + ret);
    }
    batch.getStagedSequences()
        .forEach(piece -> {
          final Sequence<?> sequence = piece.getSequence();
          sequence.setLastLogitIndiciesMap(piece.getLogitsIndiciesMap());
          sequence.setLength(sequence.getLength() + piece.getLength());
          if (!sequences.containsKey(sequence.getSequenceId())) {
            sequences.put(sequence.getSequenceId(), sequence);
          }
        });
    batch.clear();
    return this;
  }

  public int getContextSize() {
    return llamaContextParams.getnCtx();
  }

  Batch getEmbeddingsBatch() {
    return embeddingsBatch;
  }

  void setEmbeddingsBatch(final Batch embeddingsBatch) {
    this.embeddingsBatch = embeddingsBatch;
  }

  /**
   * Retrieves the logits for the highest sequence index provided in the last evaluation.
   *
   * @param sequence The sequence that was previously evaluated
   * @return A list of logit probabilities with a size of the model's vocab
   */
  public List<Float> getLogits(final Sequence<?> sequence) {
    if (sequence.getLastLogitIndicies().isEmpty()) {
      throw new LlamaApiException("Context has not yet evaluated a sequence.");
    }
    final int lastIndex = sequence.getLastLogitIndicies()
        .get(sequence.getLastLogitIndicies().size() - 1);
    return getLogitsAtIndex(sequence, lastIndex);
  }

  /**
   * Retrieves the logits for the highest index provided in the last evaluation.
   *
   * @param sequence The sequence that was previously evaluated
   * @param index    A sequence index provided to the previously evaluated batch
   * @return A list of logit probabilities with a size of the model's vocab
   */
  public List<Float> getLogitsAtIndex(final Sequence<?> sequence, final int index) {
    final Map<Integer, Integer> lastLogitIndiciesMap = sequence.getLastLogitIndiciesMap();
    if (!lastLogitIndiciesMap.containsKey(index)) {
      throw new IllegalArgumentException(
          String.format("Logit with index=%s was not generated for sequenceId=%s", index,
              sequence.getSequenceId()));
    }
    final float[] logits = llamaContext.llamaGetLogitsIth(index);
    return FloatUtil.toList(logits);
  }

  Batch getTokensBatch() {
    return tokensBatch;
  }

  void setTokensBatch(final Batch tokensBatch) {
    this.tokensBatch = tokensBatch;
  }

  public void removeSequenceSegment(final Sequence<?> sequence, final int pos0, final int pos1) {
    final int posStart = pos0 < 0 ? 0 : pos0;
    final int posEnd = pos1 < 0 ? sequence.length : pos1;
    final int segmentLength = posEnd - posStart;
    if (segmentLength > sequence.getLength()) {
      throw new IllegalArgumentException("pos1 - pos0 must be less than sequence length.");
    }
    sequence.length -= segmentLength;
    final int sequenceId = sequence.getSequenceId().getId().get(0);
    llamaContext.llamaKvCacheSeqRm(sequenceId, posStart, posEnd);
    // shift remaining tokens/embeddings to the left if necessary
    if (pos1 < sequence.getLength()) {
      llamaContext.llamaKvCacheSeqShift(sequenceId, pos1, sequence.getLength(), -1 * segmentLength);
    }
  }

  public Sampler sampler(final List<Float> logits) {
    return new Sampler(llamaContext, FloatUtil.toArray(logits));
  }

  private void validateStaged(final Batch batch) {
    final int stagedLength = batch.getStagedSequences()
        .stream()
        .mapToInt(SequencePiece::getLength)
        .sum();
    final int contextLength = sequences
        .values()
        .stream()
        .mapToInt(Sequence::getLength)
        .sum();
    final int newContextLength = stagedLength + contextLength;
    if (newContextLength > getContextSize()) {
      // TODO stop using IllegalArgumentException for literally everything
      throw new IllegalArgumentException(
          String.format("Context length would exceed contextSize, contextSize=%s, contextLength=%s",
              getContextSize(), newContextLength));
    }
  }

  private void validateState() {
    if (closed) {
      throw new IllegalStateException("Context has already been closed.");
    }
  }

}