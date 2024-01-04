package net.jllama.api;

import net.jllama.core.LlamaContext;
import net.jllama.core.LlamaTokenData;
import net.jllama.core.LlamaTokenDataArray;

public class Sampler {

  final LlamaContext llamaContext;
  final LlamaTokenDataArray candidates;

  public Sampler(final LlamaContext llamaContext, final float[] logits) {
    this.llamaContext = llamaContext;
    candidates = LlamaTokenDataArray.logitsToTokenDataArray(logits);
  }

  public Sampler applyRepetitionPenalties(final int[] previousTokens, final float repetitionPenalty, final float frequencyPenalty,
      final float presencePenalty) {
    llamaContext.llamaSampleRepetitionPenalties(candidates, previousTokens, previousTokens.length, repetitionPenalty, frequencyPenalty, presencePenalty);
    return this;
  }

  public Sampler applySoftmax() {
    llamaContext.llamaSampleSoftmax(candidates);
    return this;
  }

  public Sampler keepTopK(final int k) {
    llamaContext.llamaSampleTopK(candidates, k, 1);
    return this;
  }

  public Sampler keepTopP(final float p) {
    llamaContext.llamaSampleTopP(candidates, p, 1);
    return this;
  }

  public Sampler keepMinP(final float p) {
    llamaContext.llamaSampleMinP(candidates, p, 1);
    return this;
  }

  public Sampler applyTailFree(final float z) {
    llamaContext.llamaSampleTailFree(candidates, z, 1);
    return this;
  }

  public Sampler applyLocallyTypical(float p) {
    llamaContext.llamaSampleTypical(candidates, p, 1);
    return this;
  }

  public Sampler applyTemperature(float temperature) {
    llamaContext.llamaSampleTemp(candidates, temperature);
    return this;
  }

  public float[] getLogits() {
    final float[] logits = new float[candidates.getData().length];
    final LlamaTokenData[] tokenData = candidates.getData();
    for (int i = 0; i < candidates.getData().length; i++) {
      logits[i] = tokenData[i].getLogit();
    }
    return logits;
  }

  public int sample() {
    return llamaContext.llamaSampleToken(candidates);
  }

}
