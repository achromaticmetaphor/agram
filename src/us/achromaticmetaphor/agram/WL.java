package us.achromaticmetaphor.agram;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class WL {

  public static void main(String [] args) throws IOException {
    List<String> words = new ArrayList<String>(75290);
    Scanner scan = new Scanner(WL.class.getResourceAsStream("words"));
    while (scan.hasNext())
      words.add(scan.next());

    DataOutputStream dos = new DataOutputStream(System.out);
    dos.writeInt(words.size());
    for (String w : words)
      dos.writeUTF(w);
  }

}
