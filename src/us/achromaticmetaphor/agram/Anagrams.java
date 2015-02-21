package us.achromaticmetaphor.agram;

import java.util.Arrays;
import java.util.Comparator;

public class Anagrams implements Generator {

  private static native String [] generate_native(String s, boolean full);

  public String [] generate(String s, boolean full) {
    String [] out = generate_native(s, full);
    Arrays.sort(out, new Comparator<String>() {
      public int compare(String a, String b) {
        return a.length() < b.length() ? -1 : a.length() > b.length() ? 1 : 0;
      }
    });
    return out;
  }

  public String [] generate(String s) {
    return generate(s, false);
  }

  public boolean hasLongMode() { return true; }
  public String longLabel() { return "With permutations"; }
  public String shortLabel() { return "Without permutations"; }
  public String inputPrompt() { return "Choose characters"; }

}
