package com.mangomelancholy.llama.cpp.java.bindings;

public class Main {

  static {
    System.loadLibrary("jni-implementation");
  }
  public static void main(final String[] args) {
    final Main main = new Main();

    main.sayHello();

    final int first = 1;
    final int second = 3;
    final long result = main.sum(first, second);
    System.out.printf("%d + %d = %d\n", first, second, result);

    main.initializeLlama(false);
  }

  private native void sayHello();

  private native long sum(int first, int second);

  private native void initializeLlama(boolean useNuma);
}
