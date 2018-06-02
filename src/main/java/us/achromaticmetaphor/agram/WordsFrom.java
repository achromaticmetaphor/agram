package us.achromaticmetaphor.agram;

import java.util.ArrayList;

public class WordsFrom implements Generator {

  private String s = "";
  private boolean lng = false;
  private boolean finished = true;
  private final Wordlist wordlist;

  WordsFrom(Wordlist wl) {
    wordlist = wl;
  }

  public ArrayList<String> generate(String s) {
    return generate(s, false);
  }

  public native ArrayList<String> generate(String s, boolean reuse);

  public boolean hasLongMode() { return true; }
  public String longLabel() { return "Reuse characters"; }
  public String shortLabel() { return "Don't reuse characters"; }
  public String inputPrompt() { return "Choose characters"; }

  public synchronized boolean init(String s, boolean lng) {
    this.s = s;
    this.lng = lng;
    finished = false;
    return true;
  }

  public boolean init(String s) { return init(s, false); }
  public synchronized void uninit() { finished = true; }

  public synchronized ArrayList<String> generate(int n) {
    ArrayList<String> results = finished ? new ArrayList<>() : generate(s, lng);
    finished = true;
    return results;
  }
}
