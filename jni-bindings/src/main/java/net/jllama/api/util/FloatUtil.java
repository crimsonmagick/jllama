package net.jllama.api.util;

import java.util.ArrayList;
import java.util.List;

public class FloatUtil {

  private FloatUtil() {

  }

  public static List<Float> toList(final float[] array) {
    final List<Float> list = new ArrayList<>(array.length);
    for (int i = 0; i < array.length; i++) {
      list.add(i, array[i]);
    }
    return list;
  }

  public static float[] toArray(final List<Float> list) {
    final float[] array = new float[list.size()];
    for (int i = 0; i < array.length; i++) {
      array[i] = list.get(i);
    }
    return array;
  }

}
