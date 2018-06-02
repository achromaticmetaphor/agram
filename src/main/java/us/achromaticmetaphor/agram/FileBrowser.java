package us.achromaticmetaphor.agram;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import org.androidannotations.annotations.AfterViews;
import org.androidannotations.annotations.EActivity;
import org.androidannotations.annotations.ViewById;

import java.io.File;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;

@EActivity(R.layout.activity_listview)
public class FileBrowser extends AppCompatActivity {

  private List<String> files = new ArrayList<>(0);
  private File cwd = Environment.getExternalStorageDirectory();

  @ViewById ListView cmdlist;

  @AfterViews
  protected void load() {
    files = lsdir(cwd);
    cmdlist.setAdapter(new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, files));
    cmdlist.setOnItemClickListener((AdapterView<?> av, View v, int pos, long id) -> {
        String fn = files.get(pos);
        File file = fn.equals("../") ? cwd.getParentFile() : fn.startsWith("/") ? new File(fn) : new File(cwd, fn);
        if (file == null)
          return;
        if (file.isDirectory()) {
          files = lsdir(file);
          cmdlist.setAdapter(new ArrayAdapter<>(FileBrowser.this, android.R.layout.simple_list_item_1, files));
          cwd = file;
        } else {
          final String path = file.getAbsolutePath();
          new AlertDialog.Builder(FileBrowser.this)
            .setTitle("Confirm selection")
            .setMessage("Select " + path + " as your new wordlist?")
            .setPositiveButton("Yes",
              (DialogInterface di, int i) -> {
                Intent result = new Intent();
                result.putExtra("filename", path);
                setResult(RESULT_OK, result);
                finish();
              })
            .setNegativeButton("No", null)
            .show();
        }
    });
  }

  private List<String> lsdir(File dir) {
    List<String> files = new ArrayList<>();
    files.add(dir.getAbsolutePath());
    files.add("../");
    File[] ls = dir.listFiles();
    Arrays.sort(ls, (File a, File b) -> a.getName().compareTo(b.getName()));
    for (File f : ls)
      if (f.canRead() && !f.isHidden())
        files.add(f.isDirectory() ? f.getName() + "/" : f.getName());
    return files;
  }
}
