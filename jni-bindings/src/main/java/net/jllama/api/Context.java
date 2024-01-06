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

public class Context {

  public enum SequenceType {
    EMBEDDING,
    TOKEN
  }

  Context(final LlamaContext llamaContext, final LlamaContextParams llamaContextParams) {
    this.llamaContext = llamaContext;
    this.llamaContextParams = llamaContextParams;
    this.sequences = new HashMap<>();
  }

  private final Map<SequenceId, Sequence> sequences;
  private Batch embeddingsBatch;
  private Batch tokensBatch;
  LlamaContext llamaContext;
  private LlamaContextParams llamaContextParams;

  public int getContextSize() {
    return llamaContextParams.getnCtx();
  }

  public LlamaContext getLlamaContext() {
    return llamaContext;
  }

  public BatchSpecifier batch() {
    return new FluentBatch(this);
  }

  public Sampler sampler(List<Float> logits) {
    return new Sampler(llamaContext,  FloatUtil.toArray(logits));
  }

  public Context evaluate(final Batch batch) {
    validateStaged(batch);
    final int ret = llamaContext.llamaDecode(batch.prepare());
    if (ret != 0) {
      throw new LlamaApiException("Initial decode failed with returnValue=" + ret);
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

  /**
   * Retrieves the logits for the highest index provided in the last evaluation.
   *
   * @param sequence The sequence that was previously evaluated
   * @param index A sequence index provided to the previously evaluated batch
   * @return A list of logit probabilities with a size of the model's vocab
   */
  public List<Float> getLogitsAtIndex(final Sequence<?> sequence, final int index) {
    final Map<Integer, Integer> lastLogitIndiciesMap = sequence.getLastLogitIndiciesMap();
    if (!lastLogitIndiciesMap.containsKey(index)) {
      throw new IllegalArgumentException(String.format("Logit with index=%s was not generated for sequenceId=%s", index, sequence.getSequenceId()));
    }
    final float[] logits = llamaContext.llamaGetLogitsIth(index);
    return FloatUtil.toList(logits);
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
          String.format("Context length would exceed contextSize, contextSize=%s, contextLength=%s", getContextSize(), newContextLength));
    }
  }

  Batch getEmbeddingsBatch() {
    return embeddingsBatch;
  }

  void setEmbeddingsBatch(final Batch embeddingsBatch) {
    this.embeddingsBatch = embeddingsBatch;
  }

  Batch getTokensBatch() {
    return tokensBatch;
  }

  void setTokensBatch(final Batch tokensBatch) {
    this.tokensBatch = tokensBatch;
  }

}