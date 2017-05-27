package us.achromaticmetaphor.agram;

import java.util.ArrayList;

public class Anagrams implements Generator {

  private byte[] handle = null;
  private final Wordlist wordlist;

  public Anagrams(Wordlist wl) {
    wordlist = wl;
  }

  public native ArrayList<String> generate(String s);

  public ArrayList<String> generate(String s, boolean full) {
    return generate(s);
  }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Choose characters"; }

  public synchronized native boolean init(String s);
  public synchronized native ArrayList<String> generate(int n);
  public synchronized native void uninit();

  public boolean init(String s, boolean lng) { return init(s); }
}
