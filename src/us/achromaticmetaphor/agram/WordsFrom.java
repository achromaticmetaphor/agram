package us.achromaticmetaphor.agram;

import java.util.List;

public class WordsFrom implements Generator {

  public List<String> generate(String s) {
    return generate(s, false);
  }

  public native List<String> generate(String s, boolean reuse);

  public boolean hasLongMode() { return true; }
  public String longLabel() { return "Reuse characters"; }
  public String shortLabel() { return "Don't reuse characters"; }
  public String inputPrompt() { return "Choose characters"; }

}
