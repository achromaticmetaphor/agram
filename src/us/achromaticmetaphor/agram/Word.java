package us.achromaticmetaphor.agram;

import java.util.ArrayList;
import java.util.Random;

public class Word {

  private static final Random random = new Random();

  private static native String pick_native(int n);
  private static native int get_nwords();

  public static String pick() {
    return pick_native(random.nextInt(get_nwords()));
  }

  public static ArrayList<String> pick(int n) {
    ArrayList<String> words = new ArrayList<String>(n);
    try {
      for (int i = 0; i < n; i++)
        words.add(pick());
    }
      catch (IllegalArgumentException iae) {}
    return words;
  }

}
