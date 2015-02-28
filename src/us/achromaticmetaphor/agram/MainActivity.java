package us.achromaticmetaphor.agram;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.text.Normalizer;

public class MainActivity extends Activity {

  private static String menuAbout = "About";
  private static final int REQUEST_FILEBROWSER = 1;
  private static final String builtinWordlist = "SCOWL (built-in)";
  private static final String selectedWordlistKey = "wordlist.label";
  private static final String wordlistsFilename = "wordlists";

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

  private Set<String> getWordlists() {
    Set<String> wordlists = new HashSet<String>();
    wordlists.add(builtinWordlist);
    try {
      DataInputStream dis = new DataInputStream(new BufferedInputStream(new FileInputStream(new File(getFilesDir(), wordlistsFilename))));
      int len = dis.readInt();
      for (int i = 0; i < len; i++)
        wordlists.add(dis.readUTF());
      dis.close();
    }
    catch (IOException e) {}
    return wordlists;
  }

  private void setWordlists(Collection<String> wordlists) {
    try {
      DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(new File(getFilesDir(), wordlistsFilename))));
      dos.writeInt(wordlists.size());
      for (String s : wordlists)
        dos.writeUTF(s);
      dos.close();
    }
    catch (IOException e) {}
  }

  private void enrollWordlist(String label) {
    getSharedPreferences("us.achromaticmetaphor.agram", MODE_PRIVATE).edit().putString(selectedWordlistKey, label).commit();
    Set<String> wordlists = getWordlists();
    if (! wordlists.contains(label)) {
      wordlists.add(label);
      setWordlists(wordlists);
    }
  }

  private void wordlistFail(String label) {
    AlertDialog.Builder build = new AlertDialog.Builder(this);
    build.setTitle("Error");
    build.setMessage("Failed to load wordlist: " + label);
    build.setPositiveButton("OK", null);
    build.show();
  }

  private void chooseWordlist() {
    Set<String> wordlists = getWordlists();
    final CharSequence [] wordlistsArray = wordlists.toArray(new CharSequence [0]);
    Arrays.sort(wordlistsArray);
    final int selected [] = {Arrays.binarySearch(wordlistsArray, getSharedPreferences("us.achromaticmetaphor.agram", MODE_PRIVATE).getString(selectedWordlistKey, builtinWordlist))};
    if (selected[0] < 0)
      selected[0] = -1;

    AlertDialog.Builder build = new AlertDialog.Builder(this);
    build.setTitle("Choose wordlist");
    build.setSingleChoiceItems(wordlistsArray, selected[0], new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface di, int i) {
        selected[0] = i;
      }
    });
    build.setPositiveButton("OK", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface di, int i) {
        loadWordlist("", (String) wordlistsArray[selected[0]]);
      }
    });
    build.setNeutralButton("Choose file", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface di, int i) {
        startActivityForResult(new Intent(MainActivity.this, FileBrowser.class), REQUEST_FILEBROWSER);
      }
    });
    build.setNegativeButton("Cancel", null);
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
      chooseWordlist();
    return true;
  }

  @Override
  protected void onActivityResult(int rcode, int result, Intent data) {
    if (result == RESULT_OK && rcode == REQUEST_FILEBROWSER) {
      final String filename = data.getStringExtra("us.achromaticmetaphor.agram.FileBrowser.filename");
      AlertDialog.Builder build = new AlertDialog.Builder(this);
      final EditText edit = new EditText(this);
      edit.setText(new File(filename).getName());
      build.setTitle("Enter label for wordlist.");
      build.setView(edit);
      build.setPositiveButton("OK", new DialogInterface.OnClickListener() {
        public void onClick(DialogInterface di, int i) {
          String label = edit.getText().toString();
          if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD)
            label = Normalizer.normalize(label, Normalizer.Form.NFC);
          loadWordlist(filename, label);
        }
      });
      build.setNegativeButton("Cancel", null);
      build.show();
    }
  }

}
