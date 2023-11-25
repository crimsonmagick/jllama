package net.jllama.llama.cpp.java.bindings;

import java.util.function.BiConsumer;
import net.jllama.llama.cpp.java.bindings.exceptions.ResourceNotFoundException;
import net.jllama.llama.cpp.java.bindings.util.DllExtractor;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class LlamaCpp {

  final static Logger log = LogManager.getLogger(LlamaCpp.class);

  static {
    loadJniImplementation();
  }

  private static void loadJniImplementation() {
    final String libraryName = "jni-implementation";
    try {
      loadFromOs(libraryName);
    } catch (final UnsatisfiedLinkError e) {
      log.info("Unable to load library from OS with libraryName={}", libraryName, e);
      loadFromResources(libraryName);
    }
  }

  private static void loadFromOs(final String libraryName) {
    log.info("Attempting to load library using OS path, libraryName={}", libraryName);
    System.loadLibrary(libraryName);
  }

  private static void loadFromResources(final String libraryName) {
    try {
      log.info("Attempting to load library from from resources, libraryName={}", libraryName);
      final String dllPath = DllExtractor.extract(libraryName);
      System.load(dllPath);
    } catch (final ResourceNotFoundException e) {
      log.error("Unable to locate library with libraryName={}", libraryName, e);
      throw e;
    } catch (final UnsatisfiedLinkError e) {
      log.info("Unable to load library from resources with libraryName={}", libraryName, e);
      throw e;
    }
  }

  public static native void loadLibrary();

  public static native void closeLibrary();

  public static native void llamaBackendInit(boolean useNuma);

  public static native void llamaBackendFree();

  public static native LlamaModel loadModel(byte[] pathModel, LlamaModelParams params);

  public static native float[] llamaGetLogits(LlamaContext context);

  public static native int llamaSampleToken(LlamaContext context, LlamaTokenDataArray candidates);
  public static native int llamaSampleTokenGreedy(LlamaContext context, LlamaTokenDataArray candidates);

  public static native void llamaSampleRepetitionPenalty(LlamaContext ctx, LlamaTokenDataArray candidates, int[] lastTokens, float penalty);

  public static native void llamaSampleFrequencyAndPresencePenalties(LlamaContext context, LlamaTokenDataArray candidates, int[] lastTokens, float alphaFrequency, float alphaPresence);

  public static native void llamaSampleSoftMax(LlamaContext context, LlamaTokenDataArray candidates);

  public static native void llamaSampleTopK(LlamaContext context, LlamaTokenDataArray candidates, int k, long minKeep);

  public static native void llamaSampleTopP(LlamaContext context, LlamaTokenDataArray candidates, float p, long minKeep);

  public static native void llamaSampleTailFree(LlamaContext context, LlamaTokenDataArray candidates, float z, long minKeep);

  public static native int llamaTokenBos(LlamaContext context);

  public static native int llamaTokenEos(LlamaContext context);

  public static native int llamaTokenNl(LlamaContext context);

  public static native void llamaLogSet(BiConsumer<LlamaLogLevel, byte[]> llamaLogCallback);

  public static native long llamaTimeUs();

  public static native LlamaContextParams llamaContextDefaultParams();

  public static native void llamaSampleTypical(LlamaContext llamaContext, LlamaTokenDataArray candidates, float p, int minKeep);

  public static native void llamaSampleTemperature(LlamaContext llamaContext, LlamaTokenDataArray candidates, float temp);

//  // NEW BATCH STUFF
//  LlamaBatch llamaBatchInit(int nTokens, int embd, int nSeqMax);
//  void llamaBatchFree(LlamaBatch batch);
//
//  int llamaDecode(LlamaContext ctx, LlamaBatch batch);

}
