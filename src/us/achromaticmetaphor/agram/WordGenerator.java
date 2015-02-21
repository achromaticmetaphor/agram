package us.achromaticmetaphor.agram;

public class WordGenerator implements Generator, Refreshable {

  @Override
  public String [] generate(String s) {
    int n = 1;
    try {
      n = Integer.parseInt(s);
    }
      catch (NumberFormatException nfe) {}
    return Word.pick(n);
  }

  @Override
  public String [] generate(String s, boolean b) {
    return generate(s);
  }

  @Override
  public boolean hasLongMode() { return false; }

  @Override
  public String longLabel() { return ""; }

  @Override
  public String shortLabel() { return "Generate"; }

  @Override
  public String inputPrompt() { return "Number of words"; }

}
