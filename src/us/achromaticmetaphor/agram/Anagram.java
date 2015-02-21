package us.achromaticmetaphor.agram;

public class Anagram implements Generator {

  private static native String [] generate_native(String s);

  public String [] generate(String s) {
    return generate_native(s);
  }

  public String [] generate(String s, boolean b) {
    return generate(s);
  }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Choose characters"; }

}
