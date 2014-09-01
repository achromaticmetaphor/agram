package us.achromaticmetaphor.agram;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class WordsFrom implements Generator {

  public List<String> generate(String s) {
    return generate(s, false);
  }

  public List<String> generate(String s, boolean reuse) {
    List<String> out = new ArrayList<String>();
    LCWC target = new LCWC(s);
    if (reuse)
      target.max_counts();
    for (LCWC lcwc : Words.words)
      if (lcwc.is_within(target))
        out.add(lcwc.str);
    Collections.sort(out, new Comparator<String>() {
      public int compare(String a, String b) {
        return a.length() > b.length() ? -1 : a.length() < b.length() ? 1 : a.compareTo(b);
      }
    });
    return out;
  }

}
