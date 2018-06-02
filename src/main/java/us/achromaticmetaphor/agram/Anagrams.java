package us.achromaticmetaphor.agram;

import java.util.ArrayList;

public class Anagrams implements Generator {

  private byte[] handle = null;
  private final Wordlist wordlist;

  Anagrams(Wordlist wl) { wordlist = wl; }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Choose characters"; }

  private static synchronized native byte[] initState(String s,
                                                      byte[] handle);
  private static synchronized native ArrayList<String>
  generate(int n, ArrayList<String> alist, byte[] handle);
  private static synchronized native void uninit(byte[] handle);

  public boolean init(String s) {
    handle = initState(s, wordlist.wordlist_handle);
    return handle != null;
  }

  public void uninit() {
    uninit(handle);
    handle = null;
  }

  public ArrayList<String> generate(int n, ArrayList<String> alist) {
    return generate(n, alist, handle);
  }

  public ArrayList<String> generate(int n) {
    return generate(n, new ArrayList<>(n));
  }

  public boolean init(String s, boolean lng) { return init(s); }
}
