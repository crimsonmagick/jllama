package net.jllama.api;

import java.nio.charset.StandardCharsets;
import net.jllama.core.LlamaModel;

public class Tokenizer {
  private final LlamaModel llamaModel;

  public Tokenizer(final LlamaModel llamaModel) {
    this.llamaModel = llamaModel;
  }

  public int[] tokenize(final String toTokenize) {
    return tokenize(toTokenize, false, false);
  }

  public int[] tokenize(final String toTokenize, boolean addBos, boolean special) {
    final int maxLength = toTokenize.length();
    final int[] temp = new int[maxLength];
    int length = llamaModel.llamaTokenize(toTokenize.getBytes(StandardCharsets.UTF_8), temp, maxLength, addBos, special);
    final int[] ret = new int[length];
    System.arraycopy(temp, 0, ret, 0, length);
    return ret;
  }
}
