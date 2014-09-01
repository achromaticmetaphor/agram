package us.achromaticmetaphor.agram;

import android.os.AsyncTask;

import java.util.List;

public class AsyncGenerate extends AsyncTask<String, Void, List<String> []> {

  public interface Listener {
    public void onFinished(List<String> result);
  }

  private Generator gen;
  private boolean b;
  private boolean bset;
  private Listener listen;

  public AsyncGenerate(Generator gen, boolean b, Listener listen) {
    this.gen = gen;
    this.listen = listen;
    this.b = b;
    bset = true;
  }

  public AsyncGenerate(Generator gen, Listener listen) {
    this.gen = gen;
    this.listen = listen;
    bset = false;
  }

  @Override
  protected List<String> [] doInBackground(String... s) {
    List<String> [] results = new List [s.length];
    for (int i = 0; i < results.length; i++)
      results[i] = bset ? gen.generate(s[i], b) : gen.generate(s[i]);
    return results;
  }

  @Override
  protected void onPostExecute(List<String> [] list) {
    for (List<String> l : list)
      listen.onFinished(l);
  }

}
