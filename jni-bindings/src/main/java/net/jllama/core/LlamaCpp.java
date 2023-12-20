package net.jllama.core;

import java.util.function.BiConsumer;
import net.jllama.core.exceptions.ResourceNotFoundException;
import net.jllama.core.util.DllExtractor;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class LlamaCpp {

  final static Logger log = LogManager.getLogger(LlamaCpp.class);

  private LlamaCpp() {

  }

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

  public static native void llamaLogSet(BiConsumer<LlamaLogLevel, byte[]> llamaLogCallback);

  public static native long llamaTimeUs();

}
