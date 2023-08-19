package com.mangomelancholy.llama.cpp.java.bindings;

import java.lang.management.ManagementFactory;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class Main {

  static final int GENERATED_TOKEN_COUNT = 50;

  static {
    final String jvmName = ManagementFactory.getRuntimeMXBean().getName();
    final String pid = jvmName.split("@")[0];
    System.out.printf("pid=%s%n", pid);
    System.loadLibrary("jni-implementation");
  }

  public static void main(final String[] args) {
    try {
      final LlamaManager llamaManager = new LlamaManagerJNIImpl();
      final String modelPath =
          "C:\\Users\\welby\\workspace\\ai\\llama-cpp-java-bindings\\models\\llama-2-7b\\ggml-model-q4_0.bin";
      llamaManager.loadLibrary();
      llamaManager.llamaBackendInit(true);
      final LlamaContextParams llamaContextParams = generateContextParams();
      final LlamaOpaqueModel llamaOpaqueModel = llamaManager.llamaLoadModelFromFile(
          modelPath.getBytes(StandardCharsets.UTF_8), llamaContextParams);
      final LlamaOpaqueContext llamaOpaqueContext =
          llamaManager.llamaLoadContextWithModel(llamaOpaqueModel, llamaContextParams);

      final String stringToTokenize = "Hello there, my name is Fred. What is yours?";
      final byte[] toTokenize = stringToTokenize.getBytes(StandardCharsets.UTF_8);
      final int maxTokenCount = stringToTokenize.length();
      final int[] tokensTemp = new int[maxTokenCount];
      final int tokenCount = llamaManager.llamaTokenizeWithModel(llamaOpaqueModel, toTokenize, tokensTemp, maxTokenCount, true);
      final int[] tokens = new int[tokenCount];
      System.arraycopy(tokensTemp, 0, tokens, 0, tokenCount);

      final int threads = Runtime.getRuntime().availableProcessors() + 1;

      List<Integer> generatedTokens = new ArrayList<>(GENERATED_TOKEN_COUNT);

      llamaManager.llamaEval(llamaOpaqueContext, tokens, tokenCount, 0, threads);
      float[] logits = llamaManager.llamaGetLogits(llamaOpaqueContext);
      LlamaTokenDataArray tokenDataArray = LlamaTokenDataArray.logitsToTokenDataArray(logits);
      int previousToken = llamaManager.llamaSampleTokenGreedy(llamaOpaqueContext, tokenDataArray);
      generatedTokens.add(previousToken);
      for (int i = 1; i < GENERATED_TOKEN_COUNT; i++) {
        final int res = llamaManager.llamaEval(llamaOpaqueContext, new int[]{previousToken}, tokenCount, i,
            threads);
        if (res != 0) {
          throw new RuntimeException("Non zero response from eval");
        }
        logits = llamaManager.llamaGetLogits(llamaOpaqueContext);
        tokenDataArray = LlamaTokenDataArray.logitsToTokenDataArray(logits);
        previousToken = llamaManager.llamaSampleTokenGreedy(llamaOpaqueContext, tokenDataArray);
        generatedTokens.add(previousToken);
      }

      final String generatedText = generatedTokens.stream()
          .map(token -> new String(llamaManager.llamaTokenToStr(llamaOpaqueContext, token),
              StandardCharsets.UTF_8))
          .collect(Collectors.joining());
      System.out.println("Llama says: " + generatedText);

      llamaManager.llamaBackendFree();
      llamaManager.closeLibrary();
    } catch (RuntimeException e) {
      System.out.println("Fatal exception occurred, exceptionMessage=" + e.getMessage());
    }
  }

  private static LlamaContextParams generateContextParams() {
    final LlamaContextParams contextParams = new LlamaContextParams();
    contextParams.setSeed(0xFFFFFFFF);
    contextParams.setnCtx(512);
    contextParams.setnBatch(512);
    contextParams.setnGqa(1);
    contextParams.setRmsNormEps(5e-6f);
    contextParams.setnGpuLayers(0);
    contextParams.setMainGpu(0);
    contextParams.setTensorSplit(null);
    contextParams.setRopeFreqBase(10000.0f);
    contextParams.setRopeFreqScale(1.0f);
    contextParams.setProgressCallback(null);
    contextParams.setProgressCallbackUserData(null);
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
