package net.jllama.api;

import net.jllama.core.LlamaModel;
import net.jllama.core.LlamaModelParams;

public class ModelConfigurer {

  public class ParametrizedLoader {

    private ParametrizedLoader() {
      llamaModelParams = new LlamaModelParams();
    }

    private ParametrizedLoader(final LlamaModelParams llamaModelParams) {
      this.llamaModelParams = llamaModelParams;
    }

    private final LlamaModelParams llamaModelParams;
    private String path;

    public Model load() {
      return llamaApi.loadModel(path, llamaModelParams);
    }

    public ParametrizedLoader path(final String path) {
      this.path = path;
      return this;
    }

    public ParametrizedLoader gpuLayerCount(final int gpuLayerCount) {
      llamaModelParams.setnGpuLayers(gpuLayerCount);
      return this;
    }

    public ParametrizedLoader mainGpuIndex(final int mainGpuIndex) {
      llamaModelParams.setMainGpu(mainGpuIndex);
      return this;
    }

    public ParametrizedLoader tensorSplit(final float[] tensorSplit) {
      llamaModelParams.setTensorSplit(tensorSplit);
      return this;
    }

    public ParametrizedLoader vocabOnly(final boolean vocabOnly) {
      llamaModelParams.setVocabOnly(vocabOnly);
      return this;
    }

    public ParametrizedLoader useMmap(final boolean useMmap) {
      llamaModelParams.setUseMmap(useMmap);
      return this;
    }

    public ParametrizedLoader useMlock(final boolean useMlock) {
      llamaModelParams.setUseMlock(useMlock);
      return this;
    }

  }

  ModelConfigurer(final Llama llamaApi) {
    this.llamaApi = llamaApi;
  }

  private final Llama llamaApi;

  public ParametrizedLoader withDefaults() {
    return new ParametrizedLoader(LlamaModel.llamaModelDefaultParams());
  }

  public ParametrizedLoader with() {
    return new ParametrizedLoader();
  }

}
