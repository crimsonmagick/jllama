package net.jllama.core;

public enum LlamaLogLevel {
  ERROR(2), WARN(3), INFO(4);

  LlamaLogLevel(final int level) {
    this.level = level;
  }

  private final int level;

  public int getLevel() {
    return level;
  }
}
