package net.jllama.api.util;

import java.util.ArrayList;
import java.util.List;

public class IntegerUtil {

  private IntegerUtil() {

  }

  public static List<Integer> toList(final int[] array) {
    final List<Integer> list = new ArrayList<>(array.length);
    for (int i = 0; i < array.length; i++) {
      list.add(i, array[i]);
    }
    return list;
  }

  public static int[] toArray(final List<Integer> list) {
    final int[] array = new int[list.size()];
    for (int i = 0; i < array.length; i++) {
      array[i] = list.get(i);
    }
    return array;
  }

}
