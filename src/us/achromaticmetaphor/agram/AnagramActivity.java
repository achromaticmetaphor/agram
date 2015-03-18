package us.achromaticmetaphor.agram;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

import java.util.ArrayList;
import java.util.List;
import java.text.Normalizer;

public class AnagramActivity extends Activity {

  private Generator gen;
  private String input = "";
  private ArrayList<String> shareList;

  @Override
  protected void onCreate(Bundle state) {
    super.onCreate(state);
    setContentView(R.layout.activity_listview);
    gen = (Generator) getIntent().getSerializableExtra("generator");
    shareList = state == null ? null : state.getStringArrayList("shareList");
    input = state == null ? "" : state.getString("input");
    if (input == null)
      input = "";
    if (shareList == null) {
      shareList = new ArrayList<String>();
      promptCharacters();
    }
    else {
      ArrayAdapter adapter = new MonoAdapter(AnagramActivity.this, android.R.layout.simple_list_item_1, shareList);
      ((ListView) findViewById(R.id.cmdlist)).setAdapter(adapter);
    }
  }

  public void refresh(boolean lng) {
    final ProgressDialog pdia = ProgressDialog.show(AnagramActivity.this, "Generating", "Please wait", true, false);
    (new AsyncGenerate(gen, lng, new AsyncGenerate.Listener() {
      public void onFinished(ArrayList<String> result) {
        ArrayAdapter adapter = new MonoAdapter(AnagramActivity.this, android.R.layout.simple_list_item_1, result);
        ((ListView) findViewById(R.id.cmdlist)).setAdapter(adapter);
        shareList = result;
        pdia.dismiss();
      }
    })).execute(input);
  }

  private void share() {
    StringBuilder sb = new StringBuilder();
    for (String s : shareList)
      if (sb.length() >= 102400)
        break;
      else
        sb.append(s).append("\n");
    Intent send = new Intent();
    send.setAction(Intent.ACTION_SEND);
    send.putExtra(Intent.EXTRA_TEXT, sb.toString());
    send.setType("text/plain");
    startActivity(Intent.createChooser(send, null));
  }

  private void fixin(EditText et) {
    input = et.getText().toString();
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD)
      input = Normalizer.normalize(input, Normalizer.Form.NFC);
  }

  public void promptCharacters() {
    AlertDialog.Builder builder = new AlertDialog.Builder(this);
    final EditText edit = new EditText(this);
    edit.setText(input);
    edit.selectAll();
    builder.setView(edit);
    builder.setTitle(gen.inputPrompt());
    builder.setPositiveButton(gen.shortLabel(), new DialogInterface.OnClickListener () {
      public void onClick(DialogInterface di, int button) {
        fixin(edit);
        refresh(false);
      }
    });
    if (gen.hasLongMode())
      builder.setNeutralButton(gen.longLabel(), new DialogInterface.OnClickListener () {
        public void onClick(DialogInterface di, int button) {
          fixin(edit);
          refresh(true);
        }
      });
    builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener () {
      public void onClick(DialogInterface di, int button) {}
    });
    builder.show();
  }

  private static MenuItem addMenuItem(Menu menu, String title) {
    MenuItem mi = menu.add(title);
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
      mi.setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
    return mi;
  }

  private static MenuItem addMenuItem(Menu menu, String title, int ic) {
    MenuItem mi = addMenuItem(menu, title);
    mi.setIcon(ic);
    return mi;
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    super.onCreateOptionsMenu(menu);
    addMenuItem(menu, "Choose characters", R.drawable.ic_action_new);
    if (gen instanceof Refreshable)
      addMenuItem(menu, "Refresh", R.drawable.ic_action_refresh);
    addMenuItem(menu, "Share", R.drawable.ic_action_share);
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem mi) {
    super.onOptionsItemSelected(mi);
    if (mi.getTitle().equals("Choose characters"))
      promptCharacters();
    if (mi.getTitle().equals("Refresh"))
      refresh(false);
    if (mi.getTitle().equals("Share"))
      share();
    return true;
  }

  @Override
  protected void onSaveInstanceState(Bundle state) {
    state.putStringArrayList("shareList", shareList);
    state.putString("input", input);
  }

  private static class MonoAdapter extends ArrayAdapter<String> {

    public MonoAdapter(Context c, int tid, List<String> lst) {
      super(c, tid, lst);
    }

    @Override
    public View getView(int pos, View cv, ViewGroup p) {
      View v = super.getView(pos, cv, p);
      ((TextView) v.findViewById(android.R.id.text1)).setTypeface(Typeface.MONOSPACE);
      return v;
    }

  }

}
