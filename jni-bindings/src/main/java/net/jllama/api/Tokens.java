package net.jllama.api;

import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.stream.Collectors;
import net.jllama.api.util.IntegerUtil;
import net.jllama.core.LlamaModel;
import net.jllama.core.exceptions.LlamaCppException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Tokens {

  final static Logger log = LogManager.getLogger(Tokens.class);
  private final LlamaModel llamaModel;

  Tokens(final LlamaModel llamaModel) {
    this.llamaModel = llamaModel;
  }

  public String detokenize(final List<Integer> tokens) {
    return tokens.stream()
        .map(this::detokenize)
        .collect(Collectors.joining());
  }

  public String detokenize(final int token) {
    byte[] buf = new byte[8];
    int length = llamaModel.llamaDetokenize(token, buf);
    if (length < 0) {
      final int size = Math.abs(length);
      buf = new byte[size];
      length = llamaModel.llamaDetokenize(token, buf);
    }
    if (length < 0) {
      throw new RuntimeException(
          "Unable to allocate a large enough buffer for detokenized string length.");
    }
    return new String(buf, 0, length, StandardCharsets.UTF_8);
  }

  public int bos() {
    return llamaModel.llamaTokenBos();
  }

  public int eos() {
    return llamaModel.llamaTokenEos();
  }

  public int nl() {
    return llamaModel.llamaTokenNl();
  }

  public List<Integer> tokenize(final String toTokenize) {
    return tokenize(toTokenize, false, false);
  }

  public List<Integer> tokenize(final String toTokenize, boolean addBos, boolean special) {
    final int estimatedLength = toTokenize.length() + (addBos ? 1 : 0);
    final int[] temp = new int[estimatedLength];
    final int reqLength = llamaModel.llamaTokenize(toTokenize.getBytes(StandardCharsets.UTF_8), temp,
        estimatedLength, addBos, special);
    final int length;
    if (reqLength < 0) {
      log.trace(
          () -> String.format("Reserved %d ints for estimatedLength, but requiredLength was %d",
              estimatedLength, reqLength));
      length = Math.abs(reqLength);
    } else {
      length = reqLength;
    }
    final int[] tokenized = new int[length];
    System.arraycopy(temp, 0, tokenized, 0, length);
    return IntegerUtil.toList(tokenized);
  }
}
