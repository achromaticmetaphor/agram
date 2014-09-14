package us.achromaticmetaphor.agram;

import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Anagrams implements Generator {

  static { Native.load(); }

  private static native String [] generate_native(String s, boolean full);

  public List<String> generate(String s, boolean full) {
    List<String> out = Arrays.asList(generate_native(s, full));
    Collections.sort(out, new Comparator<String>() {
      public int compare(String a, String b) {
        return a.length() < b.length() ? -1 : a.length() > b.length() ? 1 : a.compareTo(b);
      }
    });
    return out;
  }

  public List<String> generate(String s) {
    return generate(s, false);
  }

  public boolean hasLongMode() { return true; }
  public String longLabel() { return "With permutations"; }
  public String shortLabel() { return "Without permutations"; }
  public String inputPrompt() { return "Choose characters"; }

}
