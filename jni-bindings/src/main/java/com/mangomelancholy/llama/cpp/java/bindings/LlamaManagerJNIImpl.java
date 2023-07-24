package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaManagerJNIImpl implements LlamaManager {

  public native void loadLibrary();

  public native void closeLibrary();

  public native void llamaBackendInit(boolean useNuma);
  public native void llamaBackendFree();
}
