package net.jllama.llama.cpp.java.bindings;

public class LlamaTokenDataArray {

  LlamaTokenData[] data;
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
    return llamaTokenDataArray;
  }

  public LlamaTokenData[] getData() {
    return data;
  }

  public void setData(LlamaTokenData[] data) {
    this.data = data;
  }

  public boolean isSorted() {
    return sorted;
  }

  public void setSorted(boolean sorted) {
    this.sorted = sorted;
  }
}
