package net.jllama.llama.cpp.java.bindings;

public class Sequence {

  private int id;
  private int length;
  private Integer lastLogitPosition;
  private boolean evaluatedLastBatch;

  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public int getLength() {
    return length;
  }

  public void setLength(int length) {
    this.length = length;
  }

  public Integer getLastLogitPosition() {
    return lastLogitPosition;
  }

  public void setLastLogitPosition(Integer lastLogitPosition) {
    this.lastLogitPosition = lastLogitPosition;
  }

  public boolean isEvaluatedLastBatch() {
    return evaluatedLastBatch;
  }

  public void setEvaluatedLastBatch(boolean evaluatedLastBatch) {
    this.evaluatedLastBatch = evaluatedLastBatch;
  }
}
