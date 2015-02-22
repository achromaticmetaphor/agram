package us.achromaticmetaphor.agram;

public class WordsFrom implements Generator {

  public String [] generate(String s) {
    return generate(s, false);
  }

  public String [] generate(String s, boolean reuse) {
    return Native.codepoints(generate_native(Native.codepoints(s), reuse));
  }

  private static native int [] [] generate_native(int [] s, boolean reuse);

  public boolean hasLongMode() { return true; }
  public String longLabel() { return "Reuse characters"; }
  public String shortLabel() { return "Don't reuse characters"; }
  public String inputPrompt() { return "Choose characters"; }

}
