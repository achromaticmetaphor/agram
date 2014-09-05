package us.achromaticmetaphor.agram;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.view.Menu;
import android.view.MenuItem;

import java.util.List;

public class AnagramActivity extends Activity {

  private Generator gen;
  private String input = "";

  @Override
  protected void onCreate(Bundle state) {
    super.onCreate(state);
    setContentView(R.layout.activity_listview);
    gen = (Generator) getIntent().getSerializableExtra("generator");
    promptCharacters();
  }

  public void refresh() {
    final ProgressDialog pdia = ProgressDialog.show(AnagramActivity.this, "Generating", "Please wait", true, false);
    (new AsyncGenerate(gen, new AsyncGenerate.Listener() {
      public void onFinished(List<String> result) {
        ArrayAdapter adapter = new ArrayAdapter(AnagramActivity.this, android.R.layout.simple_list_item_1, result);
        ((ListView) findViewById(R.id.cmdlist)).setAdapter(adapter);
        pdia.dismiss();
      }
    })).execute(input);
  }

  public void promptCharacters() {
    AlertDialog.Builder builder = new AlertDialog.Builder(this);
    final EditText edit = new EditText(this);
    edit.setText(input);
    edit.selectAll();
    builder.setView(edit);
    builder.setTitle("Choose characters");
    builder.setPositiveButton("Confirm", new DialogInterface.OnClickListener () {
      public void onClick(DialogInterface di, int button) {
        input = edit.getText().toString().toLowerCase().replaceAll("[^abcdefghijklmnopqrstuvwxyz1234567890]", "");
        refresh();
      }
    });
    builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener () {
      public void onClick(DialogInterface di, int button) {}
    });
    builder.show();
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    super.onCreateOptionsMenu(menu);
    menu.add("Choose characters");
    if (gen instanceof Refreshable)
      menu.add("Refresh");
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem mi) {
    super.onOptionsItemSelected(mi);
    if (mi.getTitle().equals("Choose characters"))
      promptCharacters();
    if (mi.getTitle().equals("Refresh"))
      refresh();
    return true;
  }
}
