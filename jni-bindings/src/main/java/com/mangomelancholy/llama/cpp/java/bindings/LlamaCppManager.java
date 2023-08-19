package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaCppManager {

  private static final LlamaCpp singleton = new LlamaCppJNIImpl();
  private LlamaCppManager() {}

  public static LlamaCpp getLlamaCpp() {
    return singleton;
  }

}
