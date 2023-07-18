package com.mangomelancholy.llama.cpp.java.bindings;

public class Main {

  static {
    System.loadLibrary("jni-implementation");
  }
  public static void main(final String[] args) {
    new Main().sayHello();
  }

  private native void sayHello();
}
