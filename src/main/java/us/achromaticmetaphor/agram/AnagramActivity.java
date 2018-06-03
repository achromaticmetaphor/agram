package us.achromaticmetaphor.agram;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Typeface;
import android.support.v7.app.AppCompatActivity;
import android.widget.AbsListView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

import org.androidannotations.annotations.AfterViews;
import org.androidannotations.annotations.Background;
import org.androidannotations.annotations.EActivity;
import org.androidannotations.annotations.Extra;
import org.androidannotations.annotations.InstanceState;
import org.androidannotations.annotations.UiThread;
import org.androidannotations.annotations.ViewById;

import java.util.ArrayList;
import java.util.List;
import java.text.Normalizer;

@EActivity(R.layout.activity_listview)
public class AnagramActivity
    extends AppCompatActivity implements AbsListView.OnScrollListener {

  private static final int SCROLL_BY = 128;

  @ViewById ListView cmdlist;

  @Extra protected Generator generator;

  @InstanceState protected Generator gen;
  @InstanceState protected String input = "";
  @InstanceState protected ArrayList<String> shareList;
  @InstanceState protected boolean longMode = false;
  private ArrayAdapter adapter;
  private ProgressDialog pdia = null;
  private boolean filling = true;

  @AfterViews
  protected void load() {
    if (gen == null)
      gen = generator;
    if (input == null)
      input = "";
    if (shareList == null) {
      shareList = new ArrayList<>();
      promptCharacters();
    } else {
      ArrayAdapter adapter =
          new MonoAdapter(AnagramActivity.this,
                          android.R.layout.simple_list_item_1, shareList);
      cmdlist.setAdapter(adapter);
      cmdlist.setOnScrollListener(this);
    }
  }

  @UiThread
  protected void dismissDialog() {
    if (pdia != null)
      pdia.dismiss();
    pdia = null;
  }

  @Background
  protected void initGen() {
    gen.init(input, longMode);
    dismissDialog();
    scroll();
  }

  @Background
  protected void scroll() {
    updateOutput(gen.generate(SCROLL_BY));
    filling = false;
  }

  @UiThread
  protected void updateOutput(ArrayList<String> data) {
    shareList.addAll(data);
    adapter.notifyDataSetChanged();
  }

  public void refresh() {
    filling = true;
    pdia = ProgressDialog.show(AnagramActivity.this, "Generating",
                               "Please wait", true, false);
    shareList = new ArrayList<>();
    adapter = new MonoAdapter(AnagramActivity.this,
                              android.R.layout.simple_list_item_1, shareList);
    cmdlist.setAdapter(adapter);
    cmdlist.setOnScrollListener(this);
    initGen();
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
    input =
        Normalizer.normalize(et.getText().toString(), Normalizer.Form.NFC);
  }

  public void promptCharacters() {
    final EditText edit = new EditText(this);
    edit.setText(input);
    edit.selectAll();
    AlertDialog.Builder builder =
        new AlertDialog.Builder(this)
            .setView(edit)
            .setTitle(gen.inputPrompt())
            .setPositiveButton(gen.shortLabel(),
                               (DialogInterface di, int button) -> {
                                 fixin(edit);
                                 longMode = false;
                                 refresh();
                               })
            .setNegativeButton("Cancel", null);
    if (gen.hasLongMode())
      builder.setNeutralButton(gen.longLabel(),
                               (DialogInterface di, int button) -> {
                                 fixin(edit);
                                 longMode = true;
                                 refresh();
                               });
    builder.show();
  }

  private static MenuItem addMenuItem(Menu menu, String title) {
    MenuItem mi = menu.add(title);
    mi.setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
    return mi;
  }

  private static void addMenuItem(Menu menu, String title, int ic) {
    addMenuItem(menu, title).setIcon(ic);
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
      refresh();
    if (mi.getTitle().equals("Share"))
      share();
    return true;
  }

  public void onScrollStateChanged(AbsListView v, int state) {}

  public void onScroll(AbsListView v, int first, int visible, int total) {
    if (!filling)
      if (total - (first + visible) < (SCROLL_BY / 4)) {
        filling = true;
        scroll();
      }
  }

  private static class MonoAdapter extends ArrayAdapter<String> {

    MonoAdapter(Context c, int tid, List<String> lst) { super(c, tid, lst); }

    @Override
    public View getView(int pos, View cv, ViewGroup p) {
      View v = super.getView(pos, cv, p);
      ((TextView) v.findViewById(android.R.id.text1))
          .setTypeface(Typeface.MONOSPACE);
      return v;
    }
  }
}
