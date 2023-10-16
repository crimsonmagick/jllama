package net.jllama.llama.cpp.java.bindings;

import java.util.function.Consumer;

public class LlamaModelParams {
    private int nGpuLayers;
    private int mainGpu;
    private float[] tensorSplit;
    private Consumer<Float> progressCallback;
    private boolean vocabOnly;
    private boolean useMmap;
    private boolean useMlock;

    public int getnGpuLayers() {
        return nGpuLayers;
    }

    public void setnGpuLayers(int nGpuLayers) {
        this.nGpuLayers = nGpuLayers;
    }

    public int getMainGpu() {
        return mainGpu;
    }

    public void setMainGpu(int mainGpu) {
        this.mainGpu = mainGpu;
    }

    public float[] getTensorSplit() {
        return tensorSplit;
    }

    public void setTensorSplit(float[] tensorSplit) {
        this.tensorSplit = tensorSplit;
    }

    public Consumer<Float> getProgressCallback() {
        return progressCallback;
    }

    public void setProgressCallback(Consumer<Float> progressCallback) {
        this.progressCallback = progressCallback;
    }

    public boolean isVocabOnly() {
        return vocabOnly;
    }

    public void setVocabOnly(boolean vocabOnly) {
        this.vocabOnly = vocabOnly;
    }

    public boolean isUseMmap() {
        return useMmap;
    }

    public void setUseMmap(boolean useMmap) {
        this.useMmap = useMmap;
    }

    public boolean isUseMlock() {
        return useMlock;
    }

    public void setUseMlock(boolean useMlock) {
        this.useMlock = useMlock;
    }
}
