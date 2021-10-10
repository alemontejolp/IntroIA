package ia;

import java.awt.EventQueue;
import java.util.Random;
import javax.swing.JFrame;
import java.awt.Color;

public class Model {

  public static void main(String[] args) {
    Random random = new Random();
    int width=200;
    int height=200;
    int simtime = 5000000;
    int numagents=10;
    int numcorpses = 5000;
    boolean infLoop = false;
    
    Environment e = new Environment(width, height);
    //Añade a los agentes al entorno
    for(int i = 0; i<numagents;i++){
      e.addAgents(new Agent(random.nextInt(width), random.nextInt(height)));
    }

    //Coloca los objetos asegurandose de que no se repiten las posiciones.
    for(int i = 0; i < numcorpses; i++) {
      int x = random.nextInt(width);
      int y = random.nextInt(height);

      Corpse c = e.getCorpsetAt(x, y);
      if(c == null) {
        int id = generateId(3);
        Color color = pickColor(id);
        e.addCorpse(new Corpse(x, y, id, color));
        //System.out.println("typeid = " + id + " color = " + color);
      } else {
        i--;
      }
    }

    System.out.println("Objetos y agentes cargados.");
    
    EventQueue.invokeLater(new Runnable() {
      public void run() {
          try {
            JFrame frame = new JFrame();
            frame.pack();
            frame.setSize(width, height);
            frame.setTitle("Hormigas");
            frame.getContentPane().add(e);
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setVisible(true);
            
          } catch (Exception e) {
            e.printStackTrace();
          }
        }
      });
    
    //Inicia la simulación simtime pasos
    int count = 0;
    for(int i = 1; i <= simtime || infLoop; i++){
      e.step();
      //i % 500000 == 0
      //i == 50000 || i == 1000000 || i == 5000000
      if(i % 10000 == 0) {
        e.repaint();
        System.out.println("Refresco: " + (++count) + ". Iteración: " + i);
      }
    }
  }

  public static int generateId(int n) {
    return (int)(Math.random() * 10) % n + 1;
  }

  public static Color pickColor(int n) {
    if(n < 1 || n > 3) {
      throw new Error("Sólo se permiten numeros en el intervalo [1, 3].");
    }
    Color c = null;
    switch(n) {
      case 1:
        c =  Color.blue;
        break;
      case 2:
        c =  Color.green;
        break;
      case 3:
        c =  Color.red;
        break;
    }
    return c;
  }
}
