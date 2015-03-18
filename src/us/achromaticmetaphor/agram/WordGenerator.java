package us.achromaticmetaphor.agram;

import java.util.ArrayList;

public class WordGenerator implements Generator, Refreshable {

  public ArrayList<String> generate(String s) {
    int n = 1;
    try {
      n = Integer.parseInt(s);
    }
      catch (NumberFormatException nfe) {}
    return Word.pick(n);
  }

  public ArrayList<String> generate(String s, boolean b) {
    return generate(s);
  }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Number of words"; }

}
