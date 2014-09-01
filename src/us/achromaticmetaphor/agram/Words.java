package us.achromaticmetaphor.agram;

import java.io.DataInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Words {

  public static List<LCWC> words;

  static {
    words = new ArrayList<LCWC>(0);
    try {
      DataInputStream dis = new DataInputStream(Words.class.getResourceAsStream("word_list.precomp"));
      int nwords = dis.readInt();
      words = new ArrayList<LCWC>(nwords);
      for (int i = 0; i < nwords; i++)
        words.add(LCWC.DISread(dis));
    }
      catch (IOException ioe) {}
  }

}
