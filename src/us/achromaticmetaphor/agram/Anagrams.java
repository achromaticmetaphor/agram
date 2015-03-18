package us.achromaticmetaphor.agram;

import java.util.Collections;
import java.util.Comparator;
import java.util.ArrayList;

public class Anagrams implements Generator {

  private static native ArrayList<String> generate_native(String s);

  public ArrayList<String> generate(String s) {
    ArrayList<String> out = generate_native(s);
    Collections.sort(out, new Comparator<String>() {
      public int compare(String a, String b) {
        return a.length() < b.length() ? -1 : a.length() > b.length() ? 1 : 0;
      }
    });
    return out;
  }

  public ArrayList<String> generate(String s, boolean full) {
    return generate(s);
  }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Choose characters"; }

}
