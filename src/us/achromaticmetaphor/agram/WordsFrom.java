package us.achromaticmetaphor.agram;

public class WordsFrom implements Generator {

  public String [] generate(String s) {
    return generate(s, false);
  }

  public String [] generate(String s, boolean reuse) {
    return generate_native(s, reuse);
  }

  private static native String [] generate_native(String s, boolean reuse);

  public boolean hasLongMode() { return true; }
  public String longLabel() { return "Reuse characters"; }
  public String shortLabel() { return "Don't reuse characters"; }
  public String inputPrompt() { return "Choose characters"; }

}
