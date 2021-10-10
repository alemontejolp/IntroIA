package ia;

import java.awt.Color;
import java.awt.Graphics;

public class Corpse {
  private int x;
  private int y;
  private int typeid;
  private Color color;

  //Constructor.
  public Corpse(int x, int y, int typeid, Color c) {
    this.x = x;
    this.y = y;
    this.typeid = typeid;
    this.color = c;
  }

  public int getX() {
    return x;
  }

  public int getY() {
    return y;
  }

  public int getTypeid() {
    return typeid;
  }

  public void setCoords(int x, int y) {
    this.x = x;
    this.y = y;
  }

  //Dibuja el objeto.
  public void draw(Graphics g) {
    if(x < 0 || y < 0)
      return;

    g.setColor(color); 
    g.fillRect(x, y, 1, 1);
  }
}
