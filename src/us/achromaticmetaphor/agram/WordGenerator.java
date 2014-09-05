package us.achromaticmetaphor.agram;

import java.util.List;

public class WordGenerator implements Generator, Refreshable {

  @Override
  public List<String> generate(String s) {
    int n = 1;
    try {
      n = Integer.parseInt(s);
    }
      catch (NumberFormatException nfe) {}
    return Word.pick(n);
  }

  @Override
  public List<String> generate(String s, boolean b) {
    return generate(s);
  }
}
