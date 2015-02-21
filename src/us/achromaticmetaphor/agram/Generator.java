package us.achromaticmetaphor.agram;

import java.io.Serializable;

interface Generator extends Serializable {
  public String [] generate(String s);
  public String [] generate(String s, boolean lng);
  public boolean hasLongMode();
  public String longLabel();
  public String shortLabel();
  public String inputPrompt();
}
