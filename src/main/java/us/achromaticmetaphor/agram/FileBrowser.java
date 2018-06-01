package us.achromaticmetaphor.agram;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.io.File;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class FileBrowser extends AppCompatActivity {

  private List<String> files = new ArrayList<String>(0);
  private File cwd = Environment.getExternalStorageDirectory();

  public static final String filenameKey = "us.achromaticmetaphor.agram.FileBrowser.filename";

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_listview);
    files = lsdir(cwd);
    final ListView listView = (ListView) findViewById(R.id.cmdlist);
    listView.setAdapter(new ArrayAdapter(this, android.R.layout.simple_list_item_1, files));
    listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
      public void onItemClick(AdapterView<?> av, View v, int pos, long id) {
        String fn = files.get(pos);
        File file = fn.equals("../") ? cwd.getParentFile() : fn.startsWith("/") ? new File(fn) : new File(cwd, fn);
        if (file == null)
          return;
        if (file.isDirectory()) {
          files = lsdir(file);
          listView.setAdapter(new ArrayAdapter(FileBrowser.this, android.R.layout.simple_list_item_1, files));
          cwd = file;
        } else {
          final String path = file.getAbsolutePath();
          AlertDialog.Builder build = new AlertDialog.Builder(FileBrowser.this);
          build.setTitle("Confirm selection");
          build.setMessage("Select " + path + " as your new wordlist?");
          build.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface di, int i) {
              Intent result = new Intent();
              result.putExtra(filenameKey, path);
              setResult(RESULT_OK, result);
              finish();
            }
          });
          build.setNegativeButton("No", null);
          build.show();
        }
      }
    });
  }

  private List<String> lsdir(File dir) {
    List<String> files = new ArrayList<String>();
    files.add(dir.getAbsolutePath());
    files.add("../");
    File[] ls = dir.listFiles();
    Arrays.sort(ls, new Comparator<File>() {
      public int compare(File a, File b) {
        return a.getName().compareTo(b.getName());
      }
    });
    for (File f : ls) {
      if (f.canRead())
        if (!f.isHidden())
          files.add(f.isDirectory() ? f.getName() + "/" : f.getName());
    }
    return files;
  }
}
