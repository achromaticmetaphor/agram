package us.achromaticmetaphor.agram;

import android.Manifest;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;

import org.androidannotations.annotations.AfterViews;
import org.androidannotations.annotations.Background;
import org.androidannotations.annotations.EActivity;
import org.androidannotations.annotations.OnActivityResult;
import org.androidannotations.annotations.UiThread;
import org.androidannotations.annotations.ViewById;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.text.Normalizer;

@EActivity(R.layout.activity_listview)
public class MainActivity extends AppCompatActivity {

  @ViewById ListView cmdlist;

  private static String menuAbout = "About";
  private static final int REQUEST_FILEBROWSER = 1;
  private static final int REQUEST_PERMISSION_READ_EXTERNAL_STORAGE = 2;
  private static final String builtinWordlist = "SCOWL (built-in)";
  private static final String selectedWordlistKey = "wordlist.label";
  private static final byte wordlistsVersion = 2;
  private static final String wordlistsFilename = Wordlist.transformLabel("wordlists", wordlistsVersion);

  private Wordlist wordlist;
  private ProgressDialog pdia;

  @AfterViews
  protected void load() {
    final String[] cmds = new String[] {"Single-word anagrams", "Multi-word anagrams", "Random words", "Contained words"};
    final ArrayAdapter adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, cmds);
    cmdlist.setAdapter(adapter);
    cmdlist.setOnItemClickListener((AdapterView<?> av, View v, int pos, long id) ->
      AnagramActivity_.intent(this)
        .generator(pos == 0 ? new Anagram(wordlist) : pos == 1 ? new Anagrams(wordlist) : pos == 2 ? new WordGenerator(wordlist) : new WordsFrom(wordlist))
        .start());
    wordlist = new Wordlist();
    SharedPreferences prefs = getSharedPreferences("us.achromaticmetaphor.agram", MODE_PRIVATE);
    String selectedWordlist = prefs.getString(selectedWordlistKey, builtinWordlist);
    if (!getWordlists().contains(selectedWordlist))
      selectedWordlist = builtinWordlist;
    loadWordlist("", selectedWordlist);
  }

  private void loadWordlist(String filename, String label) {
    pdia = ProgressDialog.show(this, "Loading word list", "Please wait", true, false);
    readWordlist(filename, label);
  }

  @Background
  protected void readWordlist(String filename, final String label) {
    try {
      final InputStream words = filename.equals("") ? getResources().getAssets().open("words") : new FileInputStream(filename);
      if (wordlist.load(getFilesDir(), words, label))
        enrollWordlist(label);
      else
        wordlistFail(label);
    } catch (IOException ioe) {
      wordlistFail(label);
    }
  }

  private Set<String> getWordlists() {
    Set<String> wordlists = new HashSet<>();
    wordlists.add(builtinWordlist);
    try {
      DataInputStream dis = new DataInputStream(new BufferedInputStream(new FileInputStream(new File(getFilesDir(), wordlistsFilename))));
      int len = dis.readInt();
      for (int i = 0; i < len; i++)
        wordlists.add(dis.readUTF());
      dis.close();
    } catch (IOException e) {
    }
    return wordlists;
  }

  private void setWordlists(Collection<String> wordlists) {
    try {
      DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(new File(getFilesDir(), wordlistsFilename))));
      dos.writeInt(wordlists.size());
      for (String s : wordlists)
        dos.writeUTF(s);
      dos.close();
    } catch (IOException e) {
    }
  }

  private void enrollWordlist(String label) {
    getSharedPreferences("us.achromaticmetaphor.agram", MODE_PRIVATE).edit().putString(selectedWordlistKey, label).commit();
    Set<String> wordlists = getWordlists();
    if (!wordlists.contains(label)) {
      wordlists.add(label);
      setWordlists(wordlists);
    }
    wordlistSuccess(label);
  }

  @UiThread
  protected void wordlistSuccess(String label) {
    dismissDialog();
  }

  private void dismissDialog() {
    if (pdia != null)
      pdia.dismiss();
    pdia = null;
  }

  @UiThread
  protected void wordlistFail(String label) {
    dismissDialog();
    new AlertDialog.Builder(this)
      .setTitle("Error")
      .setMessage("Failed to load wordlist: " + label)
      .setPositiveButton("OK", null)
      .show();
  }

  private void chooseWordlist() {
    Set<String> wordlists = getWordlists();
    final CharSequence[] wordlistsArray = wordlists.toArray(new CharSequence[0]);
    Arrays.sort(wordlistsArray);
    final int selected[] = {Arrays.binarySearch(wordlistsArray, getSharedPreferences("us.achromaticmetaphor.agram", MODE_PRIVATE).getString(selectedWordlistKey, builtinWordlist))};
    if (selected[0] < 0)
      selected[0] = -1;

    new AlertDialog.Builder(this)
      .setTitle("Choose wordlist")
      .setSingleChoiceItems(wordlistsArray, selected[0], (DialogInterface di, int i) -> selected[0] = i)
      .setPositiveButton("OK", (DialogInterface di, int i) -> loadWordlist("", (String) wordlistsArray[selected[0]]))
      .setNeutralButton("Choose file", (DialogInterface di, int i) -> {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
          requestPermissions(new String[] {Manifest.permission.READ_EXTERNAL_STORAGE}, REQUEST_PERMISSION_READ_EXTERNAL_STORAGE);
        else
          FileBrowser_.intent(MainActivity.this).startForResult(REQUEST_FILEBROWSER);
      })
      .setNegativeButton("Cancel", null)
      .show();
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
      About_.intent(this).start();
    if (mi.getTitle().equals("Select wordlist"))
      chooseWordlist();
    return true;
  }

  @OnActivityResult(REQUEST_FILEBROWSER)
  protected void wordlistFileSelected(int result, @OnActivityResult.Extra String filename) {
    if (result == RESULT_OK) {
      final EditText edit = new EditText(this);
      edit.setText(new File(filename).getName());
      new AlertDialog.Builder(this)
        .setTitle("Enter label for wordlist.")
        .setView(edit)
        .setPositiveButton("OK",
          (DialogInterface di, int i) -> {
            String label = edit.getText().toString();
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD)
              label = Normalizer.normalize(label, Normalizer.Form.NFC);
            loadWordlist(filename, label);
          })
        .setNegativeButton("Cancel", null)
        .show();
    }
  }

  @Override
  public void onRequestPermissionsResult(int request, String[] perms, int[] results) {
    if (request == REQUEST_PERMISSION_READ_EXTERNAL_STORAGE && perms.length == 1 && perms[0].equals(Manifest.permission.READ_EXTERNAL_STORAGE) && results[0] == PackageManager.PERMISSION_GRANTED)
      FileBrowser_.intent(this).startForResult(REQUEST_FILEBROWSER);
  }
}
