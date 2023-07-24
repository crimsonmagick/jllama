package com.mangomelancholy.llama.cpp.java.bindings;

public interface LlamaManager {

  void initializeLlama(boolean useNuma);
  void terminateLlama();
}
