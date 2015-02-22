package us.achromaticmetaphor.agram;

import java.util.Random;

public class Word {

  private static final Random random = new Random();

  private static native int [] pick_native(int n);
  private static native int get_nwords();

  public static String pick() {
    int [] codepoints = pick_native(random.nextInt(get_nwords()));
    return new String(codepoints, 0, codepoints.length);
  }

  public static String [] pick(int n) {
    String [] words = new String [n];
    for (int i = 0; i < n; i++)
      words[i] = pick();
    return words;
  }

}
