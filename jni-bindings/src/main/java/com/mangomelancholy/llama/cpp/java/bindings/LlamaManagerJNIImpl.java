package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaManagerJNIImpl implements LlamaManager {

  public native void loadLibrary();

  public native void closeLibrary();

  public native void llamaBackendInit(boolean useNuma);
  public native void llamaBackendFree();

  public native LlamaOpaqueModel llamaLoadModelFromFile(byte[] pathModel, LlamaContextParams params);

  public native byte[] llamaTimesC(byte[] path);
  public native byte[] llamaTimesCpp(byte[] path);
}
