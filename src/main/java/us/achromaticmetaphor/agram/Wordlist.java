package us.achromaticmetaphor.agram;

import android.os.AsyncTask;
import android.util.Base64;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.Serializable;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Wordlist implements Serializable {

  public static byte currentVersion = 3;
  private static byte [] oldVersions = {2};

  private byte [] wordlist_handle = null;

  static {
    Native.load();
  }

  public interface OnCompleteListener {
    public void onComplete(boolean success);
  }

  public Wordlist() {
    loadNullWordlist();
  }

  private static native byte [] platform();
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
    }
    catch (NoSuchAlgorithmException nsae) {
      return label;
    }
  }

  public static String transformLabel(String label) {
    return transformLabel(label, currentVersion);
  }

  public void load(File filesDir, InputStream in, String label) {
    load(filesDir, in, label, null);
  }

  private static native boolean upgrade(String oldpath, String newpath, byte oldversion);

  private static boolean upgrade(File filesDir, String label) {
    for (byte ver : oldVersions) {
      File oldList = new File(filesDir, transformLabel(label, ver));
      File newList = new File(filesDir, transformLabel(label));
      if (oldList.exists() && upgrade(oldList.getAbsolutePath(), newList.getAbsolutePath(), ver))
        return true;
    }
    return false;
  }

  public void load(final File filesDir, final InputStream in, final String label, final OnCompleteListener listen) {
    (new AsyncTask<Void, Void, Boolean>() {
      protected Boolean doInBackground(Void... v) {
        synchronized (Wordlist.class) {
          File wlfile = new File(filesDir, transformLabel(label));
          if (wlfile.exists() || upgrade(filesDir, label))
            return init(wlfile.getAbsolutePath());
          else
            return init(wlfile.getAbsolutePath(), new WordlistReader(new BufferedReader(new InputStreamReader(in))));
        }
      }
      protected void onPostExecute(Boolean b) {
        if (listen != null)
          listen.onComplete(b);
      }
    }).execute();
  }
}
