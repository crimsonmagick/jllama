package com.mangomelancholy.llama.cpp.java.bindings;

public class Main {

  static {
    System.loadLibrary("jni-implementation");
  }
  public static void main(final String[] args) {
    try {
      final LlamaManager llamaManager = new LlamaManagerJNIImpl();
      llamaManager.loadLibrary();
      llamaManager.llamaBackendInit(true);
      llamaManager.llamaBackendFree();
      llamaManager.closeLibrary();
    } catch (RuntimeException e) {
      System.out.println("Fatal exception occurred, exceptionMessage=" + e.getMessage());
    }
  }

}
