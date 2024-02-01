package net.jllama.api;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import net.jllama.api.util.ByteUtil;
import net.jllama.api.util.IntegerUtil;
import net.jllama.core.LlamaModel;
import net.jllama.core.exceptions.LlamaCppException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Tokens {

  final static Logger log = LogManager.getLogger(Tokens.class);
  final private static byte UTF8_SPACE = " ".getBytes(StandardCharsets.UTF_8)[0];
  private final LlamaModel llamaModel;

  Tokens(final LlamaModel llamaModel) {
    this.llamaModel = llamaModel;
  }

  public int bos() {
    return llamaModel.llamaTokenBos();
  }

  public byte[] detokenize(final List<Integer> tokens) {
    final List<Byte> utf8Text = new ArrayList<>();
    for (int token : tokens) {
      final byte[] detokenized = detokenize(token);
      for (byte utf8piece : detokenized) {
        utf8Text.add(utf8piece);
      }
    }
    return ByteUtil.toArray(utf8Text);
  }

  public byte[] detokenize(final int token) {
    byte[] buf = new byte[8];
    int length = llamaModel.llamaDetokenize(token, buf);
    if (length < 0) {
      final int size = Math.abs(length);
      buf = new byte[size];
      length = llamaModel.llamaDetokenize(token, buf);
    }
    if (length < 0) {
      throw new RuntimeException(
          "Unable to allocate a large enough buffer for detokenized byte[] length.");
    }
    byte[] rawUtf8String = new byte[length];
    System.arraycopy(buf, 0, rawUtf8String, 0, length);
    return rawUtf8String;
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
    final int requiredLength = llamaModel.llamaTokenize(toTokenize.getBytes(StandardCharsets.UTF_8),
        temp, estimatedLength, addBos, special);
    final int[] tokenized;
    if (requiredLength < 0) {
      log.trace(
          () -> String.format("Reserved %d ints for estimatedLength, but requiredLength was %d",
              estimatedLength, requiredLength));
      final int length = Math.abs(requiredLength);
      tokenized = new int[length];
      final int secondTokenizationResult = llamaModel.llamaTokenize(
          toTokenize.getBytes(StandardCharsets.UTF_8), tokenized, length, addBos, special);
      if (secondTokenizationResult < 0) {
        final String errorMessage = String.format(
            "Reserved %d ints based on first tokenization attempt, but but second attempt failed with a return value of %d",
            estimatedLength, length);
        log.error(errorMessage);
        throw new LlamaCppException(errorMessage);
      }
    } else {
      tokenized = new int[requiredLength];
      System.arraycopy(temp, 0, tokenized, 0, requiredLength);
    }
    return IntegerUtil.toList(tokenized);
  }
}
