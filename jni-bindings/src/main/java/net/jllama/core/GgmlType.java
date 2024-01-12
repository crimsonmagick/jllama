package net.jllama.core;

import static java.util.Arrays.stream;

import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;

public enum GgmlType {
  GGML_TYPE_F32(0),
  GGML_TYPE_F16(1),
  GGML_TYPE_Q4_0(2),
  GGML_TYPE_Q4_1(3),
  GGML_TYPE_Q5_0(6),
  GGML_TYPE_Q5_1(7),
  GGML_TYPE_Q8_0(8),
  GGML_TYPE_Q8_1(9),
  GGML_TYPE_Q2_K(10),
  GGML_TYPE_Q3_K(11),
  GGML_TYPE_Q4_K(12),
  GGML_TYPE_Q5_K(13),
  GGML_TYPE_Q6_K(14),
  GGML_TYPE_Q8_K(15),
  GGML_TYPE_I8(16),
  GGML_TYPE_I16(17),
  GGML_TYPE_I32(18),
  GGML_TYPE_COUNT(19);

  private final int value;
  private static final Map<Integer, GgmlType> reverseValueMap = stream(GgmlType.values())
      .collect(Collectors.toMap(GgmlType::getValue, Function.identity()));

  public static GgmlType getType(final int value) {
    return reverseValueMap.get(value);
  }

  GgmlType(final int value) {
    this.value = value;
  }

  public int getValue() {
    return value;
  }

}
