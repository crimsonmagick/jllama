package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaManagerJNIImpl implements LlamaManager {

  public native void initializeLlama(boolean useNuma);
  public native void terminateLlama();
}
