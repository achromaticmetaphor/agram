package us.achromaticmetaphor.agram;

import java.util.ArrayList;
import java.util.Random;

public class Word {

  private static final Random random = new Random();

  private static native String pick(byte[] handle, int n);

  public static String pick(Wordlist wl) {
    return pick(wl.wordlist_handle, random.nextInt(wl.get_nwords()));
  }

  public static ArrayList<String> pick(Wordlist wl, int n) {
    ArrayList<String> words = new ArrayList<>(n);
    try {
      for (int i = 0; i < n; i++)
        words.add(pick(wl));
    } catch (IllegalArgumentException iae) {
    }
    return words;
  }
}
