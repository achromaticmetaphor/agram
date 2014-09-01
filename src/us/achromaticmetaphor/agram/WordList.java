package us.achromaticmetaphor.agram;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class WordList {

  public static String [] words;

  static {
    words = new String [0];
    try {
      DataInputStream dis = new DataInputStream(WordList.class.getResourceAsStream("word_list"));
      String [] ws = new String [dis.readInt()];
      for (int i = 0; i < ws.length; i++)
        ws[i] = dis.readUTF();
      words = ws;
    }
      catch (IOException ioe) {}
  }

  public static void main(String [] args) throws IOException {
    DataOutputStream dos = new DataOutputStream(System.out);
    dos.writeInt(words.length);
    for (String w : words)
      (new LCWC(w)).DOSwrite(dos);
  }

}
