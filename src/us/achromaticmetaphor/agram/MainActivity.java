package us.achromaticmetaphor.agram;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.StringReader;

public class MainActivity extends Activity {

  private static String menuAbout = "About";

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_listview);
    final String [] cmds = new String [] {"Single-word anagrams", "Multi-word anagrams", "Random words", "Contained words"};
    final ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, cmds);
    ListView listView = (ListView) findViewById(R.id.cmdlist);
    listView.setAdapter(adapter);
    listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
      public void onItemClick(AdapterView<?> av, View v, int pos, long id) {
        Intent intent = new Intent(MainActivity.this, AnagramActivity.class);
        intent.putExtra("generator", pos == 0 ? new Anagram() : pos == 1 ? new Anagrams() : pos == 2 ? new WordGenerator() : new WordsFrom());
        startActivity(intent);
      }
    });
    final ProgressDialog pdia = ProgressDialog.show(this, "Loading word list", "Please wait", true, false);
    (new AsyncTask<Void, Void, Void>() {
      @Override
      protected Void doInBackground(Void... v) {
        if (new File(getFilesDir(), "wordlist.k").exists())
          Native.init(new File(getFilesDir(), "wordlist").getAbsolutePath());
        else {
          try {
            Native.init(new File(getFilesDir(), "wordlist").getAbsolutePath(), new BufferedReader(new InputStreamReader(getResources().getAssets().open("words"))));
          }
          catch (IOException ioe) {
            Native.init(new File(getFilesDir(), "NULL").getAbsolutePath(), new BufferedReader(new StringReader("")));
          }
        }
        return null;
      }
      @Override
      protected void onPostExecute(Void v) {
        pdia.dismiss();
      }
    }).execute();
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    super.onCreateOptionsMenu(menu);
    MenuItem about = menu.add(menuAbout);
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
      about.setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
    about.setIcon(R.drawable.ic_action_about);
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem mi) {
    super.onOptionsItemSelected(mi);
    if (mi.getTitle().equals(menuAbout))
      startActivity(new Intent(this, About.class));
    return true;
  }

}
