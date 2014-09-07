package us.achromaticmetaphor.agram;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class MainActivity extends Activity {

  private static String menuAbout = "About";

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_listview);
    final String [] cmds = new String [] {"Anagram", "Anagrams", "Word", "WordsFrom"};
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
