package us.achromaticmetaphor.agram;

import android.os.AsyncTask;
import android.util.Base64;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Wordlist {

  static {
    Native.load();
  }

  public interface OnCompleteListener {
    public void onComplete(boolean success);
  }

  private static native byte [] platform();

  public static String transformLabel(String label) {
    try {
      MessageDigest dig = MessageDigest.getInstance("SHA-256");
      dig.update(platform());
      dig.update(label.getBytes());
      return Base64.encodeToString(dig.digest(), Base64.URL_SAFE | Base64.NO_WRAP | Base64.NO_PADDING);
    }
    catch (NoSuchAlgorithmException nsae) {
      return label;
    }
  }

  public static void load(File filesDir, InputStream in, String label) {
    load(filesDir, in, label, null);
  }

  public static void load(final File filesDir, final InputStream in, String label, final OnCompleteListener listen) {
    final String filename = transformLabel(label);
    (new AsyncTask<Void, Void, Boolean>() {
      protected Boolean doInBackground(Void... v) {
        synchronized (Wordlist.class) {
          if (new File(filesDir, filename + ".k").exists())
            return Native.init(new File(filesDir, filename).getAbsolutePath());
          else
            return Native.init(new File(filesDir, filename).getAbsolutePath(), new WordlistReader(new BufferedReader(new InputStreamReader(in))));
        }
      }
      protected void onPostExecute(Boolean b) {
        if (listen != null)
          listen.onComplete(b);
      }
    }).execute();
  }
}
