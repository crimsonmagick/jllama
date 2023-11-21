package net.jllama.llama.cpp.java.bindings;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class LlamaBatch {
  int nTokens;
  IntBuffer token;
  FloatBuffer embd;
  IntBuffer pos;
  int[][] seqId;
  int[] nSeqId; // llama.cpp is allocating memory for this, so we need to keep a reference
  byte[] logits;

}
