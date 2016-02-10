package us.achromaticmetaphor.agram;

import java.util.ArrayList;

public class WordGenerator implements Generator, Refreshable {

  private final Wordlist wordlist;
  private int wordsRemaining = -1;

  public WordGenerator(Wordlist wl) {
    wordlist = wl;
  }

  private static int getInt(String s, int def) {
    try {
      return Integer.parseInt(s);
    }
      catch (NumberFormatException nfe) {
        return def;
      }
  }

  public ArrayList<String> generate(String s) {
    return Word.pick(wordlist, getInt(s, 1));
  }

  public ArrayList<String> generate(String s, boolean b) {
    return generate(s);
  }

  public boolean hasLongMode() { return false; }
  public String longLabel() { return ""; }
  public String shortLabel() { return "Generate"; }
  public String inputPrompt() { return "Number of words"; }

  public synchronized boolean init(String s) {
    wordsRemaining = getInt(s, -1);
    return true;
  }

  public boolean init(String s, boolean lng) {
    return init(s);
  }

  public synchronized void uninit() { wordsRemaining = 0; }

  public synchronized ArrayList<String> generate(int n) {
    if (wordsRemaining >= 0) {
      n = Math.min(wordsRemaining, n);
      wordsRemaining -= n;
    }
    return Word.pick(wordlist, n);
  }

}
