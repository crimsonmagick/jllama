package com.mangomelancholy.llama.cpp.java.bindings.runner;

import com.mangomelancholy.llama.cpp.java.bindings.LlamaCpp;
import com.mangomelancholy.llama.cpp.java.bindings.LlamaOpaqueContext;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.stream.Collectors;

public class Detokenizer {

  public Detokenizer(final LlamaCpp llamaCpp) {
    this.llamaManager = llamaCpp;
  }

  private final LlamaCpp llamaManager;

  public String detokenize(List<Integer> tokens, LlamaOpaqueContext llamaOpaqueContext) {
     return tokens.stream()
        .map(token -> new String(llamaManager.llamaTokenToStr(llamaOpaqueContext, token),
            StandardCharsets.UTF_8))
        .collect(Collectors.joining());
  }

  public String detokenize(int token, LlamaOpaqueContext llamaOpaqueContext) {
    return new String(llamaManager.llamaTokenToStr(llamaOpaqueContext, token),
        StandardCharsets.UTF_8);
  }

}
