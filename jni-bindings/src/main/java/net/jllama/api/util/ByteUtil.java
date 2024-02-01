package net.jllama.api.util;

import java.util.ArrayList;
import java.util.List;

public class ByteUtil {

  private ByteUtil() {

  }

  public static List<Byte> toList(final byte[] array) {
    final List<Byte> list = new ArrayList<>(array.length);
    for (int i = 0; i < array.length; i++) {
      list.add(i, array[i]);
    }
    return list;
  }

  public static byte[] toArray(final List<Byte> list) {
    final byte[] array = new byte[list.size()];
    for (int i = 0; i < array.length; i++) {
      array[i] = list.get(i);
    }
    return array;
  }
}
