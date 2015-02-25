package us.achromaticmetaphor.agram;

import java.io.BufferedReader;
import java.io.IOException;

public class Native {

  static {
    System.loadLibrary("anagram");
  }

  public static native boolean init(String fn, BufferedReader reader) throws IOException;
  public static native boolean init(String fn);

}
