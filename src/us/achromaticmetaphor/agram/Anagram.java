package us.achromaticmetaphor.agram;

import java.util.Arrays;
import java.util.List;

public class Anagram implements Generator {

  static { Native.load(); }

  private static native String [] generate_native(String s);

  public List<String> generate(String s) {
    return Arrays.asList(generate_native(s));
  }

  public List<String> generate(String s, boolean b) {
    return generate(s);
  }

}
