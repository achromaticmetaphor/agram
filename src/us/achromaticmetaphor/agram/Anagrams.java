package us.achromaticmetaphor.agram;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Anagrams implements Generator {

  public List<LCWC> filter_wc(Iterable<LCWC> wcs, LCWC target) {
    List<LCWC> out = new ArrayList<LCWC>();
    for (LCWC lcwc : wcs)
      if (lcwc.is_within(target))
        out.add(lcwc);
    return out;
  }

  public void generate(Collection<String> out, LCWC target, String prefix, Iterable<LCWC> wcs_in, boolean full) {
    List<LCWC> wcs = filter_wc(wcs_in, target);
    for (int i = 0; i < wcs.size(); i++) {
      LCWC lcwc = wcs.get(i);
      String s = prefix.length() == 0 ? lcwc.str : prefix + " " + lcwc.str;
      if (target.is_anagram(lcwc))
        out.add(s);
      else
        generate(out, target.sub(lcwc), s, full ? wcs : wcs.subList(i, wcs.size()), full);
    }
  }

  public List<String> generate(String s, boolean full) {
    List<String> out = new ArrayList<String>();
    generate(out, new LCWC(s), "", Words.words, full);
    Collections.sort(out, new Comparator<String>() {
      public int compare(String a, String b) {
        return a.length() < b.length() ? -1 : a.length() > b.length() ? 1 : a.compareTo(b);
      }
    });
    return out;
  }

  public List<String> generate(String s) {
    return generate(s, false);
  }

}
