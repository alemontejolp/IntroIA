package ia;

import java.util.ArrayList;

public class Counting {
  private ArrayList<Integer> types;
  private ArrayList<Double> count;

  public Counting() {
    types = new ArrayList<Integer>();
    count = new ArrayList<Double>();
  }

  public void set(int typeid, double c) {
    int idx = getTypeIndex(typeid);
    if(idx != -1) {
      count.set(idx, c);
      return;
    }

    types.add(typeid);
    count.add(c);
  }

  public double get(int typeid) {
    int length = types.size();
    for(int i = 0; i < length; i++) {
      if(types.get(i) == typeid) {
        return count.get(i);
      }
    }
    return 0;
  }

  private int getTypeIndex(int typeid) {
    int length = types.size();
    for(int i = 0; i < length; i++) {
      if(types.get(i) == typeid) {
        return i;
      }
    }
    return -1;
  }
}