package ia;

import java.awt.Color;
import java.awt.Graphics;
import java.util.Random;
import java.util.ArrayList;

public class Agent {
  Random random;
  private Environment e;
  private int x;
  private int y;
  private Corpse carriedCorpse;
  private double k1;
  private double k2;
  private ArrayList<Counting> perceivedObjs;
  private int maxN;
  
  public Agent(int x, int y){
    this.x=x;
    this.y=y;
    k1 = 0.1;
    k2 = 0.3;
    random = new Random();
    carriedCorpse = null;
    perceivedObjs = new ArrayList<Counting>();
    maxN = 9 * 50; // 9 objetos por senso, por las 50 unidades de tiempo.
  }

  public void act(Environment e) {
    this.e=e;

    pickOrDrop();
    
    switch(random.nextInt(8)+1){
    case 1: x--;
    break;
    case 2: x--;
        y--;
    break;
    case 3: y--;
    break;
    case 4: x++;
          y--;
    break;
    case 5: x++;
    break;
    case 6: x++;
          y++;
      break;
    case 7: y++;
    break;
    case 8: x--;
        y++;
    break;
    }
    if(x>e.getWidth()){
      x=e.getWidth();
    }
    if(x<0){
      x=0;
    }
    if(y>e.getHeight()){
      y=e.getHeight();
    }
    if(y<0){
      y=0;
    }
  }
  
  public void draw(Graphics g){
    //Aquí va el código para dibujar al agente
    g.setColor(Color.red); 
    g.fillRect(x, y, 5, 5);
  }

  private void pickOrDrop() {
    Corpse c = e.getCorpsetAt(x, y);

    if(unleaden() && c != null) {
      //Si el agente está descargado y la celda actual ocupada.
      Counting count = e.countAdjacentCorpsesAt(x, y);
      addPerception(count);
      double f = computeFracOfPerceivedObjs(c.getTypeid());
      double pp = Math.pow(k1 / (k1 + f), 2);
      if(Math.random() <= pp) {
        carriedCorpse = c;
        carriedCorpse.setCoords(-10, -10);
      }
    } else if(!unleaden() && c == null) {
      //Si el agente esta cargando un objeto y la celda está desocupada.
      Counting count = e.countAdjacentCorpsesAt(x, y);
      addPerception(count);
      double f = computeFracOfPerceivedObjs(carriedCorpse.getTypeid());
      double pd = Math.pow(f / (k2 + f), 2);
      if(Math.random() <= pd) {
        carriedCorpse.setCoords(x, y);
        carriedCorpse = null;
      }
    }
  }

  public boolean unleaden() {
    return carriedCorpse == null;
  }

  private double computeFracOfPerceivedObjs(int typeid) {
    double n = 0;
    for(Counting count : perceivedObjs) {
      n += count.get(typeid);
    }

    return n / maxN;
  }

  private void addPerception(Counting c) {
    if(perceivedObjs.size() < 50) {
      perceivedObjs.add(c);
    } else {
      perceivedObjs.remove(0);
      perceivedObjs.add(c);
    }
  }
}
