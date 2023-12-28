package net.jllama.api;

import net.jllama.core.LlamaContext;
import net.jllama.core.LlamaContextParams;

public class ContextConfigurer {

  public class ParametrizedLoader {

    private ParametrizedLoader() {
      llamaContextParams = new LlamaContextParams();
    }

    private ParametrizedLoader(final LlamaContextParams llamaContextParams) {
      this.llamaContextParams = llamaContextParams;
    }

    private final LlamaContextParams llamaContextParams;

    public Context create() {
      return model.createContext(llamaContextParams);
    }

    public ParametrizedLoader seed(final int seed) {
      llamaContextParams.setSeed(seed);
      return this;
    }

    public ParametrizedLoader nCtx(final int nCtx) {
      llamaContextParams.setnCtx(nCtx);
      return this;
    }

    public ParametrizedLoader nBatch(final int nBatch) {
      llamaContextParams.setnBatch(nBatch);
      return this;
    }

    public ParametrizedLoader nThreads(final int nThreads) {
      llamaContextParams.setnThreads(nThreads);
      return this;
    }

    public ParametrizedLoader nThreadsBatch(final int nThreadsBatch) {
      llamaContextParams.setnThreadsBatch(nThreadsBatch);
      return this;
    }

    public ParametrizedLoader ropeScalingType(final byte ropeScalingType) {
      llamaContextParams.setRopeScalingType(ropeScalingType);
      return this;
    }

    public ParametrizedLoader ropeFreqBase(final float ropeFreqBase) {
      llamaContextParams.setRopeFreqBase(ropeFreqBase);
      return this;
    }

    public ParametrizedLoader ropeFreqScale(final float ropeFreqScale) {
      llamaContextParams.setRopeFreqScale(ropeFreqScale);
      return this;
    }

    public ParametrizedLoader yarnExtFactor(final float yarnExtFactor) {
      llamaContextParams.setYarnExtFactor(yarnExtFactor);
      return this;
    }

    public ParametrizedLoader yarnAttnFactor(final float yarnAttnFactor) {
      llamaContextParams.setYarnAttnFactor(yarnAttnFactor);
      return this;
    }

    public ParametrizedLoader yarnBetaFast(final float yarnBetaFast) {
      llamaContextParams.setYarnBetaFast(yarnBetaFast);
      return this;
    }

    public ParametrizedLoader yarnBetaSlow(final float yarnBetaSlow) {
      llamaContextParams.setYarnBetaSlow(yarnBetaSlow);
      return this;
    }

    public ParametrizedLoader yarnOrigCtx(final int yarnOrigCtx) {
      llamaContextParams.setYarnOrigCtx(yarnOrigCtx);
      return this;
    }

    public ParametrizedLoader embedding(final boolean embedding) {
      llamaContextParams.setEmbedding(embedding);
      return this;
    }

  }

  ContextConfigurer(final Model model) {
    this.model = model;
  }

  private final Model model;

  public ParametrizedLoader withDefaults() {
    return new ParametrizedLoader(LlamaContext.llamaContextDefaultParams());
  }

  public ParametrizedLoader with() {
    return new ParametrizedLoader();
  }

}
