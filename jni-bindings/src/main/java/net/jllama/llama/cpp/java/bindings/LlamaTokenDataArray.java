package net.jllama.llama.cpp.java.bindings;

public class LlamaTokenDataArray {

  LlamaTokenData[] data;
  long size;
  boolean sorted;

  public static LlamaTokenDataArray logitsToTokenDataArray(float[] logits) {
    LlamaTokenData[] llamaTokenLogits = new LlamaTokenData[logits.length];
    for (int tokenCode = 0; tokenCode < logits.length; tokenCode++) {
      final LlamaTokenData llamaTokenData = new LlamaTokenData();
      llamaTokenData.setLlamaToken(tokenCode);
      llamaTokenData.setLogit(logits[tokenCode]);
      llamaTokenLogits[tokenCode] = llamaTokenData;
    }
    LlamaTokenDataArray llamaTokenDataArray = new LlamaTokenDataArray();
    llamaTokenDataArray.setData(llamaTokenLogits);
    llamaTokenDataArray.setSize(logits.length);
    return llamaTokenDataArray;
  }

  public LlamaTokenData[] getData() {
    return data;
  }

  public void setData(LlamaTokenData[] data) {
    this.data = data;
  }

  public long getSize() {
    return size;
  }

  public void setSize(long size) {
    this.size = size;
  }

  public boolean isSorted() {
    return sorted;
  }

  public void setSorted(boolean sorted) {
    this.sorted = sorted;
  }
}
