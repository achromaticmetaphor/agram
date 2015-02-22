package us.achromaticmetaphor.agram;

public class Anagram implements Generator {

  private static native int [] [] generate_native(int [] s);

  public String [] generate(String s) {
    return Native.codepoints(generate_native(Native.codepoints(s)));
  }

  public String [] generate(String s, boolean b) {
    return generate(s);
  }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Choose characters"; }

}
