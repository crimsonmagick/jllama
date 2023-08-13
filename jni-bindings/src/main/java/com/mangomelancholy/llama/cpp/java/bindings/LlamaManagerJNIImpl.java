package com.mangomelancholy.llama.cpp.java.bindings;

public class LlamaManagerJNIImpl implements LlamaManager {
  @Override
  public native void loadLibrary();
  @Override
  public native void closeLibrary();
  @Override
  public native void llamaBackendInit(boolean useNuma);
  @Override
  public native void llamaBackendFree();
  @Override
  public native LlamaOpaqueModel llamaLoadModelFromFile(byte[] pathModel, LlamaContextParams params);
  @Override
  public native LlamaOpaqueContext llamaLoadContextWithModel(LlamaOpaqueModel opaqueModel, LlamaContextParams params);
}
