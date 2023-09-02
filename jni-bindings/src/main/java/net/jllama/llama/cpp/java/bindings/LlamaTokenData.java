package net.jllama.llama.cpp.java.bindings;

public class LlamaTokenData {
  int llamaToken;
  float logit;
  float p;

  public int getLlamaToken() {
    return llamaToken;
  }

  public void setLlamaToken(int llamaToken) {
    this.llamaToken = llamaToken;
  }

  public float getLogit() {
    return logit;
  }

  public void setLogit(float logit) {
    this.logit = logit;
  }

  public float getP() {
    return p;
  }

  public void setP(float p) {
    this.p = p;
  }
}
