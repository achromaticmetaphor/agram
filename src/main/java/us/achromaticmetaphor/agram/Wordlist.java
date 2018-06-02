package us.achromaticmetaphor.agram;

import android.util.Base64;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Serializable;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Wordlist implements Serializable {

  static { Native.load(); }

  private static final byte currentVersion = 3;
  byte[] wordlist_handle = loadNullWordlist();

  private static native byte[] platform();
  private static native byte[] init(String s, WordlistReader wl);
  private static native byte[] init(String s);
  private static native int get_nwords(byte[] handle);
  public native byte[] loadNullWordlist();

  public int get_nwords() { return get_nwords(wordlist_handle); }

  public static String transformLabel(String label, byte version) {
    try {
      MessageDigest dig = MessageDigest.getInstance("SHA-256");
      dig.update(new byte[] {version});
      dig.update(platform());
      dig.update(label.getBytes());
      return Base64.encodeToString(
          dig.digest(), Base64.URL_SAFE | Base64.NO_WRAP | Base64.NO_PADDING);
    } catch (NoSuchAlgorithmException nsae) {
      return label;
    }
  }

  private static String transformLabel(String label) {
    return transformLabel(label, currentVersion);
  }

  synchronized public boolean load(final File filesDir, final InputStream in,
                                   final String label) {
    File wlfile = new File(filesDir, transformLabel(label));
    wordlist_handle = wlfile.exists()
                          ? init(wlfile.getAbsolutePath())
                          : init(wlfile.getAbsolutePath(),
                                 new WordlistReader(new BufferedReader(
                                     new InputStreamReader(in))));
    return wordlist_handle != null;
  }
}
