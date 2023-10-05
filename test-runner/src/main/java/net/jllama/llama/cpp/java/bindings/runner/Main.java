package net.jllama.llama.cpp.java.bindings.runner;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import net.jllama.llama.cpp.java.bindings.LlamaContextParams;
import net.jllama.llama.cpp.java.bindings.LlamaCpp;
import net.jllama.llama.cpp.java.bindings.LlamaCppManager;
import net.jllama.llama.cpp.java.bindings.LlamaLogLevel;
import net.jllama.llama.cpp.java.bindings.LlamaOpaqueContext;
import net.jllama.llama.cpp.java.bindings.LlamaOpaqueModel;
import net.jllama.llama.cpp.java.bindings.LlamaTokenDataArray;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.lang.management.ManagementFactory;
import java.nio.charset.StandardCharsets;

public class Main {

  private static final String SYSTEM_PROMPT = "You are a helpful, respectful and honest assistant. Always answer as helpfully as possible. If you don't know something, answer that you do not know. Take your time and be accurate.";
  private static final String COMPLETION_PROMPT = "I love my Cat Winnie, he is such a great cat! Let me tell you more about ";

  private static final String B_INST = "<s>[INST]";
  private static final String E_INST = "[/INST]";
  private static final String B_SYS = "<<SYS>>\n";
  private static final String E_SYS = "\n<</SYS>>\n\n";

  static {
    final String jvmName = ManagementFactory.getRuntimeMXBean().getName();
    final String pid = jvmName.split("@")[0];
    System.out.printf("pid=%s%n", pid);
  }
  private static volatile String appLogLevel = System.getProperty("loglevel");
  private static LlamaCpp llamaCpp;
  private static LlamaOpaqueModel llamaOpaqueModel;
  private static LlamaOpaqueContext llamaOpaqueContext;

  public static void main(final String[] args) {
    try {
      llamaCpp = LlamaCppManager.getLlamaCpp();
      final Detokenizer detokenizer = new Detokenizer(llamaCpp);
      final String modelPath = System.getProperty("modelpath");
      llamaCpp.loadLibrary();
      llamaCpp.llamaBackendInit(true);
      llamaCpp.llamaLogSet((logLevel, message) -> {
        final Logger log = LogManager.getLogger(llamaCpp.getClass());
        final String messageText = new String(message, StandardCharsets.UTF_8);
        if ("OFF".equalsIgnoreCase(appLogLevel)) {
          return;
        }
        if (logLevel == LlamaLogLevel.INFO && "INFO".equalsIgnoreCase(appLogLevel)) {
          log.info(messageText);
        } else if (logLevel == LlamaLogLevel.WARN) {
          log.warn(messageText);
        } else {
          log.error(messageText);
        }
      });
      long timestamp1 = llamaCpp.llamaTimeUs();

      final LlamaContextParams llamaContextParams = llamaCpp.llamaContextDefaultParams();
      llamaOpaqueModel = llamaCpp.llamaLoadModelFromFile(
          modelPath.getBytes(StandardCharsets.UTF_8), llamaContextParams);
      llamaOpaqueContext =
          llamaCpp.llamaLoadContextWithModel(llamaOpaqueModel, llamaContextParams);

      long timestamp2 = llamaCpp.llamaTimeUs();

      System.out.printf("timestamp1=%s, timestamp2=%s, initialization time=%s%n", timestamp1, timestamp2, timestamp2 - timestamp1);

      final String prompt = B_INST + B_SYS + SYSTEM_PROMPT + E_SYS + "Suggest a Keto-friendly meal for dinner." + E_INST;
      final int[] tokens = tokenize(prompt, true);

      // availableProcessors is the number of logical cores - we want physical cores as our basis for thread allocation
      final int threads = Runtime.getRuntime().availableProcessors() / 2 - 1;

      System.out.print(detokenizer.detokenize(toList(tokens), llamaOpaqueContext));

      llamaCpp.llamaEval(llamaOpaqueContext, tokens, tokens.length, 0, threads);
      float[] logits = llamaCpp.llamaGetLogits(llamaOpaqueContext);
      LlamaTokenDataArray candidates = LlamaTokenDataArray.logitsToTokenDataArray(logits);
      final float temp = 0.35f;
      llamaCpp.llamaSampleTemperature(llamaOpaqueContext, candidates, temp);
      int previousToken = llamaCpp.llamaSampleToken(llamaOpaqueContext, candidates);

      System.out.print(detokenizer.detokenize(previousToken, llamaOpaqueContext));

      final List<Integer> previousTokenList = new ArrayList<>();
      previousTokenList.add(previousToken);

      for (int i = tokens.length + 1; previousToken != llamaCpp.llamaTokenEos(llamaOpaqueContext) && i < llamaContextParams.getnCtx(); i++) {
        final int res = llamaCpp.llamaEval(llamaOpaqueContext, new int[]{previousToken}, 1, i, threads);
        if (res != 0) {
          throw new RuntimeException("Non zero response from eval");
        }
        logits = llamaCpp.llamaGetLogits(llamaOpaqueContext);
        candidates = LlamaTokenDataArray.logitsToTokenDataArray(logits);
//        llamaCpp.llamaSampleRepetitionPenalty(llamaOpaqueContext, candidates, toArray(previousTokenList), 1.2f);
//        llamaCpp.llamaSampleFrequencyAndPresencePenalties(llamaOpaqueContext, candidates, toArray(previousTokenList), -0.2f, -0.2f);
//        llamaCpp.llamaSampleTopK(llamaOpaqueContext, candidates, 100, 1);
//        llamaCpp.llamaSampleSoftMax(llamaOpaqueContext, candidates);
//        llamaCpp.llamaSampleTopP(llamaOpaqueContext, candidates, 0.001f, 1);
//        llamaCpp.llamaSampleTailFree(llamaOpaqueContext, candidates, 0.5f, 1);
//        llamaCpp.llamaSampleTypical(llamaOpaqueContext, candidates, 0.5f, 1);
        llamaCpp.llamaSampleTemperature(llamaOpaqueContext, candidates, temp);
        previousToken = llamaCpp.llamaSampleToken(llamaOpaqueContext, candidates);
        previousTokenList.add(previousToken);
        System.out.print(detokenizer.detokenize(previousToken, llamaOpaqueContext));
      }

      llamaCpp.llamaFree(llamaOpaqueContext);
      llamaCpp.llamaFreeModel(llamaOpaqueModel);
      llamaCpp.llamaBackendFree();
      llamaCpp.closeLibrary();
    } catch (RuntimeException e) {
      System.out.println("Fatal exception occurred, exceptionMessage=" + e.getMessage());
    }
  }

  private static int[] tokenize(final String text, boolean addBos) {
    final int maxLength = text.length();
    final int[] temp = new int[maxLength];
    int length = llamaCpp.llamaTokenizeWithModel(llamaOpaqueModel, text.getBytes(StandardCharsets.UTF_8), temp, maxLength, addBos);
    final int[] ret = new int[length];
    System.arraycopy(temp, 0, ret, 0, length);
    return ret;
  }

  private static List<Integer> toList(int[] tokens) {
    return Arrays.stream(tokens).boxed().collect(Collectors.toList());
  }

  private static int[] toArray(List<Integer> tokenList) {
    int[] tokens = new int[tokenList.size()];
    Arrays.setAll(tokens, tokenList::get);
    return tokens;
  }

}
