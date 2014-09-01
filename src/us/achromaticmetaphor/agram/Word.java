package us.achromaticmetaphor.agram;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Word {

  public static String pick() {
    return WordList.words[new Random().nextInt(WordList.words.length)];
  }

  public static List<String> pick(int n) {
    List<String> words = new ArrayList<String>(n);
    for (int i = 0; i < n; i++)
      words.add(pick());
    return words;
  }

}
