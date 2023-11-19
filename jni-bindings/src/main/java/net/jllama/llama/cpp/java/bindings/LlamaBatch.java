package net.jllama.llama.cpp.java.bindings;

public class LlamaBatch {
  int[] token;
  int[] embd;
  int pos;
  int[][] seqId;
  int[] nSeqId; // llama.cpp is allocating memory for this, so we need to keep a reference
  byte[] logits;

  public int[] getToken() {
    return token;
  }

  public void setToken(int[] token) {
    this.token = token;
  }

  public int[] getEmbd() {
    return embd;
  }

  public void setEmbd(int[] embd) {
    this.embd = embd;
  }

  public int getPos() {
    return pos;
  }

  public void setPos(int pos) {
    this.pos = pos;
  }

  public int[][] getSeqId() {
    return seqId;
  }

  public void setSeqId(int[][] seqId) {
    this.seqId = seqId;
  }

  public int[] getnSeqId() {
    return nSeqId;
  }

  public void setnSeqId(int[] nSeqId) {
    this.nSeqId = nSeqId;
  }

  public byte[] getLogits() {
    return logits;
  }

  public void setLogits(byte[] logits) {
    this.logits = logits;
  }
}
