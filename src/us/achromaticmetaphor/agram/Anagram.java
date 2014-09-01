package us.achromaticmetaphor.agram;

import java.util.ArrayList;
import java.util.List;

public class Anagram implements Generator {

  public List<String> generate(String s) {
    List<String> out = new ArrayList<String>();
    LCWC target = new LCWC(s);
    for (LCWC lcwc : Words.words)
      if (target.is_anagram(lcwc))
        out.add(lcwc.str);
    return out;
  }

  public List<String> generate(String s, boolean b) {
    return generate(s);
  }

}
