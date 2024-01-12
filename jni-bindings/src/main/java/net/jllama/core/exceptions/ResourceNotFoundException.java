package net.jllama.core.exceptions;

public class ResourceNotFoundException extends RuntimeException {

  public ResourceNotFoundException(final String message) {
    super(message);
  }

  public ResourceNotFoundException(final String message, final Exception e) {
    super(message, e);
  }
}
