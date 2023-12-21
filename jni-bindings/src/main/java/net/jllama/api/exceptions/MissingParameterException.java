package net.jllama.api.exceptions;

public class MissingParameterException  extends RuntimeException {

  public MissingParameterException(final String message) {
    super(message);
  }
}
