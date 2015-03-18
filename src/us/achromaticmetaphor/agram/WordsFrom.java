package us.achromaticmetaphor.agram;

import java.util.ArrayList;

public class WordsFrom implements Generator {

  public ArrayList<String> generate(String s) {
    return generate(s, false);
  }

  public native ArrayList<String> generate(String s, boolean reuse);

  public boolean hasLongMode() { return true; }
  public String longLabel() { return "Reuse characters"; }
  public String shortLabel() { return "Don't reuse characters"; }
  public String inputPrompt() { return "Choose characters"; }

}
