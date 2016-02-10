package us.achromaticmetaphor.agram;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
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
  private static final int REQUEST_PERMISSION_READ_EXTERNAL_STORAGE = 2;
  private static final String builtinWordlist = "SCOWL (built-in)";
  private static final String selectedWordlistKey = "wordlist.label";
  private static final byte wordlistsVersion = 2;
  private static final String wordlistsFilename = Wordlist.transformLabel("wordlists", wordlistsVersion);

  private Wordlist wordlist;
  private ProgressDialog pdia;
  private String inprogressFilename;
  private String inprogressLabel;

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
        intent.putExtra("generator", pos == 0 ? new Anagram(wordlist) : pos == 1 ? new Anagrams(wordlist) : pos == 2 ? new WordGenerator(wordlist) : new WordsFrom(wordlist));
        startActivity(intent);
      }
    });
    wordlist = new Wordlist();
    SharedPreferences prefs = getSharedPreferences("us.achromaticmetaphor.agram", MODE_PRIVATE);
    String selectedWordlist = prefs.getString(selectedWordlistKey, builtinWordlist);
    if (! getWordlists().contains(selectedWordlist))
      selectedWordlist = builtinWordlist;
    if (savedInstanceState != null && savedInstanceState.getBoolean("inprogress"))
      loadWordlist(savedInstanceState.getString("inprogressFilename"), savedInstanceState.getString("inprogressLabel"));
    else
      loadWordlist("", selectedWordlist);
  }

  private void loadWordlist(String filename, final String label) {
    inprogressFilename = filename;
    inprogressLabel = label;
    pdia = ProgressDialog.show(this, "Loading word list", "Please wait", true, false);
    try {
      wordlist.load(getFilesDir(), filename.equals("") ? getResources().getAssets().open("words") : new FileInputStream(filename), label, new Wordlist.OnCompleteListener() {
        public void onComplete(boolean success) {
          if (success)
            enrollWordlist(label);
          else
            wordlistFail(label);
          if (pdia != null)
            pdia.dismiss();
          pdia = null;
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
    if (pdia != null)
      pdia.dismiss();
    pdia = null;
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
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
          requestPermissions(new String [] {Manifest.permission.READ_EXTERNAL_STORAGE}, REQUEST_PERMISSION_READ_EXTERNAL_STORAGE);
        else
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

  @Override
  protected void onSaveInstanceState(Bundle state) {
    if (pdia != null) {
      state.putBoolean("inprogress", true);
      state.putString("inprogressFilename", inprogressFilename);
      state.putString("inprogressLabel", inprogressLabel);
      pdia.dismiss();
      pdia = null;
    }
  }

  @Override
  public void onRequestPermissionsResult(int request, String [] perms, int [] results) {
    if (request == REQUEST_PERMISSION_READ_EXTERNAL_STORAGE && perms.length == 1
     && perms[0].equals(Manifest.permission.READ_EXTERNAL_STORAGE) && results[0] == PackageManager.PERMISSION_GRANTED)
      startActivityForResult(new Intent(MainActivity.this, FileBrowser.class), REQUEST_FILEBROWSER);
  }

}
