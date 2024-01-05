package net.jllama.api;

import java.util.ArrayList;
import java.util.List;
import net.jllama.api.util.IntegerUtil;
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

  public Sampler applyRepetitionPenalties(final List<Integer> previousTokens, final float repetitionPenalty, final float frequencyPenalty,
      final float presencePenalty) {
    llamaContext.llamaSampleRepetitionPenalties(candidates, IntegerUtil.toArray(previousTokens), previousTokens.size(), repetitionPenalty, frequencyPenalty,
        presencePenalty);
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

  public List<Float> getLogits() {
    final List<Float> logits = new ArrayList<>(candidates.getData().length);
    final LlamaTokenData[] tokenData = candidates.getData();
    for (int i = 0; i < candidates.getData().length; i++) {
      logits.add(i, tokenData[i].getLogit());
    }
    return logits;
  }

  public int sample() {
    return llamaContext.llamaSampleToken(candidates);
  }

}
