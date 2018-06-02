package us.achromaticmetaphor.agram;

import java.io.Serializable;
import java.util.ArrayList;

interface Generator extends Serializable {
  ArrayList<String> generate(String s);
  ArrayList<String> generate(String s, boolean lng);
  boolean hasLongMode();
  String longLabel();
  String shortLabel();
  String inputPrompt();
  boolean init(String s);
  boolean init(String s, boolean lng);
  ArrayList<String> generate(int n);
  void uninit();
}
