package net.jllama.core;

public class LlamaContextParams {
    private int seed; // unsigned
    private int nCtx;
    private int nBatch;
    private int nThreads;
    private int nThreadsBatch;
    private byte ropeScalingType;
    private float ropeFreqBase;
    private float ropeFreqScale;
    private float yarnExtFactor;
    private float yarnAttnFactor;
    private float yarnBetaFast;
    private float yarnBetaSlow;
    private int yarnOrigCtx;
    private boolean mulMatQ;
    private boolean f16Kv;
    private boolean logitsAll;
    private boolean embedding;

    public int getSeed() {
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

    public int getnThreads() {
        return nThreads;
    }

    public void setnThreads(int nThreads) {
        this.nThreads = nThreads;
    }

    public int getnThreadsBatch() {
        return nThreadsBatch;
    }

    public void setnThreadsBatch(int nThreadsBatch) {
        this.nThreadsBatch = nThreadsBatch;
    }

    public byte getRopeScalingType() {
        return ropeScalingType;
    }

    public void setRopeScalingType(byte ropeScalingType) {
        this.ropeScalingType = ropeScalingType;
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

    public float getYarnExtFactor() {
        return yarnExtFactor;
    }

    public void setYarnExtFactor(float yarnExtFactor) {
        this.yarnExtFactor = yarnExtFactor;
    }

    public float getYarnAttnFactor() {
        return yarnAttnFactor;
    }

    public void setYarnAttnFactor(float yarnAttnFactor) {
        this.yarnAttnFactor = yarnAttnFactor;
    }

    public float getYarnBetaFast() {
        return yarnBetaFast;
    }

    public void setYarnBetaFast(float yarnBetaFast) {
        this.yarnBetaFast = yarnBetaFast;
    }

    public float getYarnBetaSlow() {
        return yarnBetaSlow;
    }

    public void setYarnBetaSlow(float yarnBetaSlow) {
        this.yarnBetaSlow = yarnBetaSlow;
    }

    public int getYarnOrigCtx() {
        return yarnOrigCtx;
    }

    public void setYarnOrigCtx(int yarnOrigCtx) {
        this.yarnOrigCtx = yarnOrigCtx;
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

    public boolean isEmbedding() {
        return embedding;
    }

    public void setEmbedding(boolean embedding) {
        this.embedding = embedding;
    }
}
