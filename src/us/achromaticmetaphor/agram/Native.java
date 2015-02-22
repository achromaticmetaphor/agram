package us.achromaticmetaphor.agram;

import java.util.List;

public class Native {

  static {
    System.loadLibrary("anagram");
  }

  public static native boolean init(String fn, int [] [] words);
  public static native boolean init(String fn);

  public static boolean init(String fn, List<String> words) {
    return init(fn, codepoints(words));
  }

  public static int [] codepoints(String word) {
    int [] codepoints = new int [word.codePointCount(0, word.length()) + 1];
    for (int i = 0, j = 0; i < codepoints.length - 1; i++) {
      codepoints[i] = word.codePointAt(j);
      j += Character.charCount(codepoints[j]);
    }
    codepoints[codepoints.length - 1] = 0;
    return codepoints;
  }

  public static int [] [] codepoints(List<String> words) {
    int [] [] codepoints = new int [words.size()] [];
    for (int i = 0; i < words.size(); i++)
      codepoints[i] = codepoints(words.get(i));
    return codepoints;
  }

  public static String [] codepoints(int [] [] codepoints) {
    String [] out = new String [codepoints.length];
    for (int i = 0; i < out.length; i++)
      out[i] = new String(codepoints[i], 0, codepoints[i].length);
    return out;
  }

}
