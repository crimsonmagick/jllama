package net.jllama.llama.cpp.java.bindings.exceptions;

import java.io.IOException;

public class ResourceNotFoundException extends RuntimeException {

  public ResourceNotFoundException(final String message) {
    super(message);
  }

  public ResourceNotFoundException(final String message, final Exception e) {
    super(message, e);
  }
}
