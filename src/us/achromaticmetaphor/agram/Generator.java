package us.achromaticmetaphor.agram;

import java.io.Serializable;
import java.util.List;

interface Generator extends Serializable {
  public List<String> generate(String s);
  public List<String> generate(String s, boolean lng);
  public boolean hasLongMode();
}
