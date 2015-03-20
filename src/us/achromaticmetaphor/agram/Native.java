package us.achromaticmetaphor.agram;

public class Native {

  static {
    System.loadLibrary("anagram");
  }

  public static void load() {};

  public static native boolean init(String fn, WordlistReader reader);
  public static native boolean init(String fn);

}
