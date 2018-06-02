package us.achromaticmetaphor.agram;

import java.util.ArrayList;

public class Anagram implements Generator {

  private String s = "";
  private boolean finished = true;
  private final Wordlist wordlist;

  public Anagram(Wordlist wl) {
    wordlist = wl;
  }

  public native ArrayList<String> generate(String s);

  public ArrayList<String> generate(String s, boolean b) {
    return generate(s);
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
    ArrayList<String> results = finished ? new ArrayList<>() : generate(s);
    finished = true;
    return results;
  }
}
