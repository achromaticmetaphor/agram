package us.achromaticmetaphor.agram;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.method.ScrollingMovementMethod;
import android.widget.TextView;

public class AboutText extends AppCompatActivity {

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_about);
    TextView tv = (TextView) findViewById(R.id.about);
    tv.setMovementMethod(new ScrollingMovementMethod());
    tv.setText(getIntent().getIntExtra("about_text", 0));
  }
}
