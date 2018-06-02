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

  private static final byte currentVersion = 3;
  private byte[] wordlist_handle = null;

  static {
    Native.load();
  }

  Wordlist() {
    loadNullWordlist();
  }

  private static native byte[] platform();
  private native boolean init(String s, WordlistReader wl);
  private native boolean init(String s);
  public native int get_nwords();
  public native void loadNullWordlist();

  public static String transformLabel(String label, byte version) {
    try {
      MessageDigest dig = MessageDigest.getInstance("SHA-256");
      dig.update(new byte[] {version});
      dig.update(platform());
      dig.update(label.getBytes());
      return Base64.encodeToString(dig.digest(), Base64.URL_SAFE | Base64.NO_WRAP | Base64.NO_PADDING);
    } catch (NoSuchAlgorithmException nsae) {
      return label;
    }
  }

  private static String transformLabel(String label) {
    return transformLabel(label, currentVersion);
  }

  synchronized public boolean load(final File filesDir, final InputStream in, final String label) {
    File wlfile = new File(filesDir, transformLabel(label));
    if (wlfile.exists())
      return init(wlfile.getAbsolutePath());
    else
      return init(wlfile.getAbsolutePath(), new WordlistReader(new BufferedReader(new InputStreamReader(in))));
  }
}
