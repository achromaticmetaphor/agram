package us.achromaticmetaphor.agram;

import java.io.Serializable;
import java.util.ArrayList;

interface Generator extends Serializable {
  public ArrayList<String> generate(String s);
  public ArrayList<String> generate(String s, boolean lng);
  public boolean hasLongMode();
  public String longLabel();
  public String shortLabel();
  public String inputPrompt();
}
