package com.mangomelancholy.llama.cpp.java.bindings;

public class Main {

  static {
    System.loadLibrary("jni-implementation");
  }
  public static void main(final String[] args) {
    try {
      final LlamaManager llamaManager = new LlamaManagerJNIImpl();
      llamaManager.initializeLlama(true);
      llamaManager.terminateLlama();
    } catch (RuntimeException e) {
      System.out.println("OH NO!! ohNo=" + e.getMessage());
    }
  }

}
