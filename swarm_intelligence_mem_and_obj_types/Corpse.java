package ia;

import java.awt.Color;
import java.awt.Graphics;

public class Corpse {
  private int x;
  private int y;

  //Constructor.
  public Corpse(int x, int y) {
    this.x = x;
    this.y = y;
  }

  public int getX() {
    return x;
  }

  public int getY() {
    return y;
  }

  public void setCoords(int x, int y) {
    this.x = x;
    this.y = y;
  }

  //Dibuja el objeto.
  public void draw(Graphics g) {
    if(x < 0 || y < 0)
      return;

    g.setColor(Color.blue); 
    g.fillRect(x, y, 1, 1);
  }
}
