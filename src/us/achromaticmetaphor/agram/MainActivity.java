package us.achromaticmetaphor.agram;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class MainActivity extends Activity {

  private static String menuAbout = "About";
  private static final int REQUEST_FILEBROWSER = 1;
  private static final String selectedWordlistKey = "wordlist.label";

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
    SharedPreferences prefs = getSharedPreferences("us.achromaticmetaphor.agram", MODE_PRIVATE);
    loadWordlist("", prefs.getString(selectedWordlistKey, builtinWordlist));
  }

  private void loadWordlist(String filename, final String label) {
    try {
      Wordlist.load(this, filename.equals("") ? getResources().getAssets().open("words") : new FileInputStream(filename), label, new Wordlist.OnCompleteListener() {
        public void onComplete(boolean success) {
          if (success)
            enrollWordlist(label);
          else
            wordlistFail(label);
        }
      });
    }
    catch (IOException ioe) {
      wordlistFail(label);
    }
  }

  private void enrollWordlist(String label) {
    getSharedPreferences("us.achromaticmetaphor.agram", MODE_PRIVATE).edit().putString(selectedWordlistKey, label).commit();
  }

  private void wordlistFail(String label) {
    AlertDialog.Builder build = new AlertDialog.Builder(this);
    build.setTitle("Error");
    build.setMessage("Failed to load wordlist: " + label);
    build.setPositiveButton("OK", null);
    build.show();
  }

  private MenuItem addMenuItem(Menu menu, String label) {
    MenuItem mi = menu.add(label);
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
      mi.setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
    return mi;
  }

  private MenuItem addMenuItem(Menu menu, String label, int icon) {
    MenuItem mi = addMenuItem(menu, label);
    mi.setIcon(icon);
    return mi;
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    super.onCreateOptionsMenu(menu);
    addMenuItem(menu, "Select wordlist", R.drawable.ic_action_collection);
    addMenuItem(menu, menuAbout, R.drawable.ic_action_about);
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem mi) {
    super.onOptionsItemSelected(mi);
    if (mi.getTitle().equals(menuAbout))
      startActivity(new Intent(this, About.class));
    if (mi.getTitle().equals("Select wordlist"))
      startActivityForResult(new Intent(this, FileBrowser.class), REQUEST_FILEBROWSER);
    return true;
  }

  @Override
  protected void onActivityResult(int rcode, int result, Intent data) {
    if (result == RESULT_OK && rcode == REQUEST_FILEBROWSER) {
      String filename = data.getStringExtra(FileBrowser.filenameKey);
      loadWordlist(filename, filename);
    }
  }

}
