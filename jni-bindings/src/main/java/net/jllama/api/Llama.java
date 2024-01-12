package net.jllama.api;

import java.nio.charset.StandardCharsets;
import net.jllama.api.exceptions.LlamaApiException;
import net.jllama.api.exceptions.MissingParameterException;
import net.jllama.core.LlamaCpp;
import net.jllama.core.LlamaLogLevel;
import net.jllama.core.LlamaModel;
import net.jllama.core.LlamaModelParams;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Llama implements AutoCloseable {

  final private static Logger log = LogManager.getLogger(Llama.class);
  private static Llama singleton;

  private static Boolean initializedUseNuma;

  private boolean closed;

  public static Llama library() {
    return library(false);
  }
  public static synchronized Llama library(final boolean useNuma) {
    if (singleton != null) {
      if (useNuma != initializedUseNuma) {
        throw new LlamaApiException(String.format("Llama.CPP has already been initialized with useNuma=%s", initializedUseNuma));
      }
      return singleton;
    }
    LlamaCpp.loadLibrary();
    singleton = new Llama(useNuma);
    initializedUseNuma = useNuma;
    return singleton;
  }

  private static void logLllamaMessage(final LlamaLogLevel logLevel, final byte[] message) {
    final String messageText = new String(message, StandardCharsets.UTF_8);
    if (logLevel == LlamaLogLevel.INFO) {
      log.info(messageText);
    } else if (logLevel == LlamaLogLevel.WARN) {
      log.warn(messageText);
    } else {
      log.error(messageText);
    }
  }

  private Llama(final boolean useNuma) {
    initializedUseNuma = useNuma;
    LlamaCpp.loadLibrary();
    LlamaCpp.llamaBackendInit(useNuma);
    LlamaCpp.llamaLogSet(Llama::logLllamaMessage);
  }

  public ModelConfigurer newModel() {
    return new ModelConfigurer(this);
  }

  Model loadModel(final String path, final LlamaModelParams llamaModelParams) {
    if (path == null || path.isEmpty()) {
      throw new MissingParameterException("Model cannot be loaded. Path is a required parameter.");
    }
    // TODO add progress logging
    final LlamaModel llamaModel = LlamaCpp.loadModel(path.getBytes(StandardCharsets.UTF_8), llamaModelParams);
    return new Model(llamaModel);
  }

  @Override
  public synchronized void close() {
    // TODO add method for deregistering the logger (avoid a possible memory leak)
    if (closed) {
      throw new IllegalStateException("Llama has already been closed.");
    }
    LlamaCpp.llamaBackendFree();
    LlamaCpp.closeLibrary();
    initializedUseNuma = null;
    singleton = null;
    closed = true;
  }

}
