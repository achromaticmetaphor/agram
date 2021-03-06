package us.achromaticmetaphor.agram;

import android.support.annotation.Keep;

import java.io.BufferedReader;
import java.io.IOException;
import java.text.Normalizer;

@Keep
public class WordlistReader {

  private BufferedReader reader;

  WordlistReader(BufferedReader br) { reader = br; }

  public String read() {
    try {
      String s;
      do
        s = reader.readLine();
      while (s != null && s.length() == 0);
      return s != null ? Normalizer.normalize(s, Normalizer.Form.NFC) : null;
    } catch (IOException e) {
      return null;
    }
  }
}
