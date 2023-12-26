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
    private String path;

    public Context create() {
      return model.createContext(llamaContextParams);
    }

    public ParametrizedLoader seed(final int seed) {
      llamaContextParams.setSeed(seed);
      return this;
    }

    public ParametrizedLoader setnCtx(final int nCtx) {
      llamaContextParams.setnCtx(nCtx);
      return this;
    }

    public ParametrizedLoader setnBatch(final int nBatch) {
      llamaContextParams.setnBatch(nBatch);
      return this;
    }

    public ParametrizedLoader setnThreads(final int nThreads) {
      llamaContextParams.setnThreads(nThreads);
      return this;
    }

    public ParametrizedLoader setnThreadsBatch(final int nThreadsBatch) {
      llamaContextParams.setnThreadsBatch(nThreadsBatch);
      return this;
    }

    public ParametrizedLoader setRopeScalingType(final byte ropeScalingType) {
      llamaContextParams.setRopeScalingType(ropeScalingType);
      return this;
    }

    public ParametrizedLoader setRopeFreqBase(final float ropeFreqBase) {
      llamaContextParams.setRopeFreqBase(ropeFreqBase);
      return this;
    }

    public ParametrizedLoader setRopeFreqScale(final float ropeFreqScale) {
      llamaContextParams.setRopeFreqScale(ropeFreqScale);
      return this;
    }

    public ParametrizedLoader setYarnExtFactor(final float yarnExtFactor) {
      llamaContextParams.setYarnExtFactor(yarnExtFactor);
      return this;
    }

    public ParametrizedLoader setYarnAttnFactor(final float yarnAttnFactor) {
      llamaContextParams.setYarnAttnFactor(yarnAttnFactor);
      return this;
    }

    public ParametrizedLoader setYarnBetaFast(final float yarnBetaFast) {
      llamaContextParams.setYarnBetaFast(yarnBetaFast);
      return this;
    }

    public ParametrizedLoader setYarnBetaSlow(final float yarnBetaSlow) {
      llamaContextParams.setYarnBetaSlow(yarnBetaSlow);
      return this;
    }

    public ParametrizedLoader setYarnOrigCtx(final int yarnOrigCtx) {
      llamaContextParams.setYarnOrigCtx(yarnOrigCtx);
      return this;
    }

    public ParametrizedLoader setEmbedding(final boolean embedding) {
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
