package net.jllama.llama.cpp.java.bindings;

import net.jllama.llama.cpp.java.bindings.exceptions.ResourceNotFoundException;
import net.jllama.llama.cpp.java.bindings.util.DllExtractor;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.function.BiConsumer;

class LlamaCppJNIImpl implements LlamaCpp {

  final static Logger log = LogManager.getLogger(LlamaCppJNIImpl.class);

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

  @Override
  public native void loadLibrary();

  @Override
  public native void closeLibrary();

  @Override
  public native void llamaBackendInit(boolean useNuma);

  @Override
  public native void llamaBackendFree();

  @Override
  public native LlamaOpaqueModel llamaLoadModelFromFile(byte[] pathModel,
      LlamaModelParams params);

  @Override
  public native void llamaFreeModel(LlamaOpaqueModel model);

  @Override
  public native LlamaOpaqueContext llamaNewContextWithModel(LlamaOpaqueModel opaqueModel,
      LlamaContextParams params);

  @Override
  public native void llamaFree(LlamaOpaqueContext context);

  @Override
  public native int llamaTokenize(LlamaOpaqueModel model, byte[] text, int[] tokens,
      int nMaxTokens, boolean addBos);

  @Override
  public native int llamaEval(LlamaOpaqueContext context, int[] tokens, int nTokens, int nPast);

  @Override
  public native float[] llamaGetLogits(LlamaOpaqueContext context);

  @Override
  public native int llamaSampleToken(LlamaOpaqueContext context, LlamaTokenDataArray candidates);

  @Override
  public native int llamaSampleTokenGreedy(LlamaOpaqueContext context,
      LlamaTokenDataArray candidates);

  @Override
  public native int llamaTokenToPiece(LlamaOpaqueModel model, int llamaToken, byte[] buf);

  @Override
  public native void llamaSampleRepetitionPenalty(LlamaOpaqueContext ctx, LlamaTokenDataArray candidates, int[] lastTokens, float penalty);

  @Override
  public native void llamaSampleFrequencyAndPresencePenalties(LlamaOpaqueContext context, LlamaTokenDataArray candidates, int[] lastTokens, float alphaFrequency, float alphaPresence);

  @Override
  public native void llamaSampleSoftMax(LlamaOpaqueContext context, LlamaTokenDataArray candidates);

  @Override
  public native void llamaSampleTopK(LlamaOpaqueContext context, LlamaTokenDataArray candidates, int k, long minKeep);

  @Override
  public native void llamaSampleTopP(LlamaOpaqueContext context, LlamaTokenDataArray candidates, float p, long minKeep);

  @Override
  public native void llamaSampleTailFree(LlamaOpaqueContext context, LlamaTokenDataArray candidates, float z, long minKeep);

  @Override
  public native int llamaTokenBos(LlamaOpaqueContext context);

  @Override
  public native int llamaTokenEos(LlamaOpaqueContext context);

  @Override
  public native int llamaTokenNl(LlamaOpaqueContext context);

  @Override
  public native void llamaLogSet(BiConsumer<LlamaLogLevel, byte[]> llamaLogCallback);

  @Override
  public native long llamaTimeUs();

  @Override
  public native LlamaContextParams llamaContextDefaultParams();

  @Override
  public native LlamaModelParams llamaModelDefaultParams();

  @Override
  public native void llamaSampleTypical(LlamaOpaqueContext llamaOpaqueContext,
      LlamaTokenDataArray candidates, float p, int minKeep);

  @Override
  public native void llamaSampleTemperature(LlamaOpaqueContext llamaOpaqueContext,
      LlamaTokenDataArray candidates, float temp);

}
