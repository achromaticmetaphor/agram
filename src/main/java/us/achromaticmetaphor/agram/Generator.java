package us.achromaticmetaphor.agram;

import java.io.Serializable;
import java.util.ArrayList;

interface Generator extends Serializable {
  boolean hasLongMode();
  String longLabel();
  String shortLabel();
  String inputPrompt();
  boolean init(String s);
  boolean init(String s, boolean lng);
  ArrayList<String> generate(int n);
  void uninit();
}
