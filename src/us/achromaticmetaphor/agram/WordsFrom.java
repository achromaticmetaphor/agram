package us.achromaticmetaphor.agram;

import java.util.Arrays;
import java.util.List;

public class WordsFrom implements Generator {

  public List<String> generate(String s) {
    return generate(s, false);
  }

  public List<String> generate(String s, boolean reuse) {
    return Arrays.asList(generate_native(s, reuse));
  }

  private static native String [] generate_native(String s, boolean reuse);

  public boolean hasLongMode() { return true; }
  public String longLabel() { return "Reuse characters"; }
  public String shortLabel() { return "Don't reuse characters"; }
  public String inputPrompt() { return "Choose characters"; }

}
