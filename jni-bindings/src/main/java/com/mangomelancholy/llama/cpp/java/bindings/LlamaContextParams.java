package com.mangomelancholy.llama.cpp.java.bindings;

import java.util.function.Consumer;

public class LlamaContextParams {
    private int seed; // unsigned
    private int nCtx;
    private int nBatch;
    private int nGqa;
    private float rmsNormEps;
    private int nGpuLayers;
    private int mainGpu;
    private float[] tensorSplit;
    private float ropeFreqBase;
    private float ropeFreqScale;
    private Consumer<Float> progressCallback;
    private boolean lowVram;
    private boolean mulMatQ;
    private boolean f16Kv;
    private boolean logitsAll;
    private boolean vocabOnly;
    private boolean useMmap;
    private boolean useMlock;
    private boolean embedding;

    public long getSeed() {
        return seed;
    }

    public void setSeed(int seed) {
        this.seed = seed;
    }

    public int getnCtx() {
        return nCtx;
    }

    public void setnCtx(int nCtx) {
        this.nCtx = nCtx;
    }

    public int getnBatch() {
        return nBatch;
    }

    public void setnBatch(int nBatch) {
        this.nBatch = nBatch;
    }

    public int getnGqa() {
        return nGqa;
    }

    public void setnGqa(int nGqa) {
        this.nGqa = nGqa;
    }

    public float getRmsNormEps() {
        return rmsNormEps;
    }

    public void setRmsNormEps(float rmsNormEps) {
        this.rmsNormEps = rmsNormEps;
    }

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

    public float getRopeFreqBase() {
        return ropeFreqBase;
    }

    public void setRopeFreqBase(float ropeFreqBase) {
        this.ropeFreqBase = ropeFreqBase;
    }

    public float getRopeFreqScale() {
        return ropeFreqScale;
    }

    public void setRopeFreqScale(float ropeFreqScale) {
        this.ropeFreqScale = ropeFreqScale;
    }

    public Consumer<Float> getProgressCallback() {
        return progressCallback;
    }

    public void setProgressCallback(Consumer<Float> progressCallback) {
        this.progressCallback = progressCallback;
    }

    public boolean isLowVram() {
        return lowVram;
    }

    public void setLowVram(boolean lowVram) {
        this.lowVram = lowVram;
    }

    public boolean isMulMatQ() {
        return mulMatQ;
    }

    public void setMulMatQ(boolean mulMatQ) {
        this.mulMatQ = mulMatQ;
    }

    public boolean isF16Kv() {
        return f16Kv;
    }

    public void setF16Kv(boolean f16Kv) {
        this.f16Kv = f16Kv;
    }

    public boolean isLogitsAll() {
        return logitsAll;
    }

    public void setLogitsAll(boolean logitsAll) {
        this.logitsAll = logitsAll;
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

    public boolean isEmbedding() {
        return embedding;
    }

    public void setEmbedding(boolean embedding) {
        this.embedding = embedding;
    }
}
