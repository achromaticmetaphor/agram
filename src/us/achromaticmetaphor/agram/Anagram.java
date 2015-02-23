package us.achromaticmetaphor.agram;

import java.util.List;

public class Anagram implements Generator {

  public native List<String> generate(String s);

  public List<String> generate(String s, boolean b) {
    return generate(s);
  }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Choose characters"; }

}
