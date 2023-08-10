package com.mangomelancholy.llama.cpp.java.bindings;

import java.lang.management.ManagementFactory;
import java.nio.charset.StandardCharsets;

public class Main {

  static {
    final String jvmName = ManagementFactory.getRuntimeMXBean().getName();
    final String pid = jvmName.split("@")[0];
    System.out.printf("pid=%s%n", pid);
    System.loadLibrary("jni-implementation");
  }
  public static void main(final String[] args) {
    try {
      final LlamaManager llamaManager = new LlamaManagerJNIImpl();
      llamaManager.loadLibrary();
      llamaManager.llamaBackendInit(true);
      llamaManager.llamaBackendFree();
      llamaManager.closeLibrary();
      final String orig = "C:\\\u0000こんにちは\\dou\\ですか\\𝓐";
      System.out.println("original: " + orig);
      final byte[] origPathBytes = orig.getBytes(StandardCharsets.UTF_8);

      final byte[] cRetPath = llamaManager.llamaTimesC(origPathBytes);
      System.out.println("c_version: " + new String(cRetPath, StandardCharsets.UTF_8));

      final byte[] cppRetPath= llamaManager.llamaTimesCpp(origPathBytes);
      System.out.println("cpp_version: " + new String(cppRetPath, StandardCharsets.UTF_8));
    } catch (RuntimeException e) {
      System.out.println("Fatal exception occurred, exceptionMessage=" + e.getMessage());
    }
  }

}
