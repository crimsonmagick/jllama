package com.mangomelancholy.llama.cpp.java.bindings.runner;

import com.mangomelancholy.llama.cpp.java.bindings.LlamaContextParams;
import com.mangomelancholy.llama.cpp.java.bindings.LlamaCpp;
import com.mangomelancholy.llama.cpp.java.bindings.LlamaCppManager;
import com.mangomelancholy.llama.cpp.java.bindings.LlamaLogLevel;
import com.mangomelancholy.llama.cpp.java.bindings.LlamaOpaqueContext;
import com.mangomelancholy.llama.cpp.java.bindings.LlamaOpaqueModel;
import com.mangomelancholy.llama.cpp.java.bindings.LlamaTokenDataArray;
import java.lang.management.ManagementFactory;
import java.nio.charset.StandardCharsets;

public class Main {

  static {
    final String jvmName = ManagementFactory.getRuntimeMXBean().getName();
    final String pid = jvmName.split("@")[0];
    System.out.printf("pid=%s%n", pid);
    System.loadLibrary("jni-implementation");
  }
  private static volatile String appLogLevel = System.getProperty("loglevel");

  public static void main(final String[] args) {
    try {
      final LlamaCpp llamaCpp = LlamaCppManager.getLlamaCpp();
      final Detokenizer detokenizer = new Detokenizer(llamaCpp);
      final String modelPath = System.getProperty("modelpath");
      llamaCpp.loadLibrary();
      llamaCpp.llamaBackendInit(true);
      llamaCpp.llamaLogSet((logLevel, message) -> {
        final String messageText = new String(message, StandardCharsets.UTF_8);
        if ("OFF".equalsIgnoreCase(appLogLevel)) {
          return;
        }
        if (logLevel == LlamaLogLevel.INFO && "INFO".equalsIgnoreCase(appLogLevel)) {
          System.out.print("INFO:" + messageText);
        } else if (logLevel == LlamaLogLevel.WARN) {
          System.out.print("WARN: " + messageText);
        } else {
          System.out.print("ERR: " + messageText);
        }
      });
      long timestamp1 = llamaCpp.llamaTimeUs();

      final LlamaContextParams llamaContextParams = generateContextParams();
      final LlamaOpaqueModel llamaOpaqueModel = llamaCpp.llamaLoadModelFromFile(
          modelPath.getBytes(StandardCharsets.UTF_8), llamaContextParams);
      final LlamaOpaqueContext llamaOpaqueContext =
          llamaCpp.llamaLoadContextWithModel(llamaOpaqueModel, llamaContextParams);

      long timestamp2 = llamaCpp.llamaTimeUs();

      System.out.printf("timestamp1=%s, timestamp2=%s, initialization time=%s%n", timestamp1, timestamp2, timestamp2 - timestamp1);

      final String prompt = "I love the Java programming language, allow me to explain why entirely in English: ";
      final byte[] toTokenize = prompt.getBytes(StandardCharsets.UTF_8);
      final int maxTokenCount = prompt.length();
      final int[] tokensTemp = new int[maxTokenCount];
      final int tokenCount = llamaCpp.llamaTokenizeWithModel(llamaOpaqueModel, toTokenize, tokensTemp, maxTokenCount, true);
      final int[] tokens = new int[tokenCount];
      System.arraycopy(tokensTemp, 0, tokens, 0, tokenCount);

      // availableProcessors is the number of logical cores - we want physical cores as our basis for thread allocation
      final int threads = Runtime.getRuntime().availableProcessors() / 2 - 1;

      llamaCpp.llamaEval(llamaOpaqueContext, tokens, tokenCount, 0, threads);
      float[] logits = llamaCpp.llamaGetLogits(llamaOpaqueContext);
      LlamaTokenDataArray tokenDataArray = LlamaTokenDataArray.logitsToTokenDataArray(logits);
      int previousToken = llamaCpp.llamaSampleTokenGreedy(llamaOpaqueContext, tokenDataArray);
      int newline = llamaCpp.llamaTokenNl(llamaOpaqueContext);
      System.out.print(prompt);
      System.out.print(detokenizer.detokenize(previousToken, llamaOpaqueContext));
      for (int i = tokenCount + 1; i < 100; i++) {
        final int res = llamaCpp.llamaEval(llamaOpaqueContext, new int[]{previousToken}, 1, i, threads);
        if (res != 0) {
          throw new RuntimeException("Non zero response from eval");
        }
        logits = llamaCpp.llamaGetLogits(llamaOpaqueContext);
        tokenDataArray = LlamaTokenDataArray.logitsToTokenDataArray(logits);
        previousToken = llamaCpp.llamaSampleTokenGreedy(llamaOpaqueContext, tokenDataArray);
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

  private static LlamaContextParams generateContextParams() {
    final LlamaContextParams contextParams = new LlamaContextParams();
    contextParams.setSeed(0xFFFFFFFF);
    contextParams.setnCtx(512);
    contextParams.setnBatch(512);
    contextParams.setnGpuLayers(0);
    contextParams.setMainGpu(0);
    contextParams.setTensorSplit(null);
    contextParams.setRopeFreqBase(10000.0f);
    contextParams.setRopeFreqScale(1.0f);
//    contextParams.setProgressCallback(progress -> System.out.println("jProgress: " + progress));
    contextParams.setLowVram(false);
    contextParams.setMulMatQ(false);
    contextParams.setF16Kv(true);
    contextParams.setLogitsAll(false);
    contextParams.setVocabOnly(false);
    contextParams.setUseMmap(true);
    contextParams.setUseMlock(false);
    contextParams.setEmbedding(false);
    return contextParams;
  }

}
