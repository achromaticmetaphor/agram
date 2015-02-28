package us.achromaticmetaphor.agram;

import android.app.ProgressDialog;
import android.content.Context;
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

  public interface OnCompleteListener {
    public void onComplete(boolean success);
  }

  public static String transformLabel(String label) {
    try {
      MessageDigest dig = MessageDigest.getInstance("SHA-256");
      return Base64.encodeToString(dig.digest(label.getBytes()), Base64.URL_SAFE | Base64.NO_WRAP | Base64.NO_PADDING);
    }
    catch (NoSuchAlgorithmException nsae) {
      return label;
    }
  }

  public static void load(Context con, InputStream in, String label) {
    load(con, in, label, null);
  }

  public static void load(final Context con, final InputStream in, String label, final OnCompleteListener listen) {
    final String filename = transformLabel(label);
    final ProgressDialog pdia = ProgressDialog.show(con, "Loading word list", "Please wait", true, false);
    (new AsyncTask<Void, Void, Boolean>() {
      protected Boolean doInBackground(Void... v) {
        if (new File(con.getFilesDir(), filename + ".k").exists())
          return Native.init(new File(con.getFilesDir(), filename).getAbsolutePath());
        else
          return Native.init(new File(con.getFilesDir(), filename).getAbsolutePath(), new WordlistReader(new BufferedReader(new InputStreamReader(in))));
      }
      protected void onPostExecute(Boolean b) {
        pdia.dismiss();
        if (listen != null)
          listen.onComplete(b);
      }
    }).execute();
  }
}
