package com.mangomelancholy.llama.cpp.java.bindings;

import java.lang.management.ManagementFactory;
import java.nio.charset.StandardCharsets;

public class Main {

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
      llamaManager.llamaLoadModelFromFile(modelPath.getBytes(StandardCharsets.UTF_8),
          generateContextParams());
      llamaManager.llamaBackendFree();
      llamaManager.closeLibrary();
    } catch (RuntimeException e) {
      System.out.println("Fatal exception occurred, exceptionMessage=" + e.getMessage());
    }
  }

  private static LlamaContextParams generateContextParams() {
    final LlamaContextParams contextParams = new LlamaContextParams();
    contextParams.setSeed(12L);
    return contextParams;
  }

}
