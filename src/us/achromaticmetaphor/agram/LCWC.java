package us.achromaticmetaphor.agram;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

public class LCWC {

  public final String str;
  private Map<Character,Integer> counts;

  public LCWC(String s) {
    str = s;
    counts = new HashMap<Character,Integer>();
    for (char c : s.toCharArray())
      if (counts.containsKey(c))
        counts.put(c, counts.get(c) + 1);
      else
        counts.put(c, 1);
  }

  private LCWC(String s, Map<Character, Integer> c) {
    str = s;
    counts = c;
  }

  public LCWC sub(LCWC that) {
    String out = "";
    for (char c : counts.keySet()) {
      int diff = that.counts.containsKey(c) ? counts.get(c) - that.counts.get(c) : counts.get(c);
      if (diff > 0)
        for (int i = 0; i < diff; i++)
          out += c;
    }
    return new LCWC(out);
  }

  public boolean is_anagram(LCWC that) {
    return counts.equals(that.counts);
  }

  public boolean is_within(LCWC that) {
    for (char c : counts.keySet())
      if (! that.counts.containsKey(c))
        return false;
      else
        if (counts.get(c) > that.counts.get(c))
          return false;
    return true;
  }

  public void max_counts() {
    for (char c : counts.keySet())
      counts.put(c, Integer.MAX_VALUE);
  }

  public void DOSwrite(DataOutputStream dos) throws IOException {
    dos.writeUTF(str);
    Set<Character> chars = counts.keySet();
    dos.writeInt(chars.size());
    for (char c : chars) {
      dos.writeChar(c);
      dos.writeInt(counts.get(c));
    }
  }

  public static LCWC DISread(DataInputStream dis) throws IOException {
    String str = dis.readUTF();
    int nchars = dis.readInt();
    Map<Character, Integer> counts = new HashMap<Character, Integer>();
    for (int i = 0; i < nchars; i++) {
      char c = dis.readChar();
      int n = dis.readInt();
      counts.put(c, n);
    }
    return new LCWC(str, counts);
  }

}
