package us.achromaticmetaphor.agram;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Word {

  private static native String pick_native(int n);
  private static native int get_nwords();

  public static String pick() {
    return pick_native(new Random().nextInt(get_nwords()));
  }

  public static List<String> pick(int n) {
    List<String> words = new ArrayList<String>(n);
    for (int i = 0; i < n; i++)
      words.add(pick());
    return words;
  }

}
