package us.achromaticmetaphor.agram;

public class Native {

  static {
    System.loadLibrary("anagram");
  }

  public static native boolean init(String fn, String [] words);
  public static native boolean init(String fn);

}
