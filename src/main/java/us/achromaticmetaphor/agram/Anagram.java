package us.achromaticmetaphor.agram;

import java.util.ArrayList;

public class Anagram implements Generator {

  private String s = "";
  private boolean finished = true;
  private final Wordlist wordlist;

  public Anagram(Wordlist wl) { wordlist = wl; }

  private static native ArrayList<String> generate(String s, byte[] handle,
                                                   ArrayList<String> alist);

  public ArrayList<String> generate(String s, ArrayList<String> alist) {
    return generate(s, wordlist.wordlist_handle, alist);
  }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Choose characters"; }

  public synchronized boolean init(String s) {
    this.s = s;
    finished = false;
    return true;
  }

  public boolean init(String s, boolean lng) { return init(s); }
  public synchronized void uninit() { finished = true; }

  public synchronized ArrayList<String> generate(int n) {
    ArrayList<String> results =
        finished ? new ArrayList<>() : generate(s, new ArrayList<>(n));
    finished = true;
    return results;
  }
}
