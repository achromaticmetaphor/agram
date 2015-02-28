package us.achromaticmetaphor.agram;

import android.os.Build;

import java.io.BufferedReader;
import java.io.IOException;
import java.text.Normalizer;

public class WordlistReader {

  private BufferedReader reader;

  public WordlistReader(BufferedReader br) {
    reader = br;
  }

  public String read() {
    try {
      String s;
      do
        s = reader.readLine();
      while (s != null && s.length() == 0);
      return Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD && s != null ? Normalizer.normalize(s, Normalizer.Form.NFC) : s;
    }
    catch (IOException e) {
      return null;
    }
  }

}
