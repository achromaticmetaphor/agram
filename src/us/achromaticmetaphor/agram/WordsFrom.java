package us.achromaticmetaphor.agram;

import java.util.Arrays;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class WordsFrom implements Generator {

  static { Native.load(); }

  public List<String> generate(String s) {
    return generate(s, false);
  }

  public List<String> generate(String s, boolean reuse) {
    List<String> out = Arrays.asList(generate_native(s, reuse));
    Collections.sort(out, new Comparator<String>() {
      public int compare(String a, String b) {
        return a.length() > b.length() ? -1 : a.length() < b.length() ? 1 : a.compareTo(b);
      }
    });
    return out;
  }

  private static native String [] generate_native(String s, boolean reuse);

  public boolean hasLongMode() { return true; }

}
