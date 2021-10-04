package ia;

import java.awt.Color;
import java.awt.Graphics;
import java.util.ArrayList;

import javax.swing.JPanel;

public class Environment extends JPanel {

	private static final long serialVersionUID = 1L;
	
  ArrayList<Agent> agents = new ArrayList<Agent>();
	ArrayList<Corpse> corpses = new ArrayList<Corpse>();
    
  public Environment(int width,int height) {
    setSize(width, height);
    setBackground(Color.white);
  }
    
  @Override
  public void paint(Graphics g) {
    super.paint(g);
    //iterar los objetos a dibujar en el entorno
    for(Agent a : agents) {
    	a.draw(g);
    }

		for(Corpse c : corpses) {
    	c.draw(g);
    }
  }
    
  public void addAgents(Agent a) {
    agents.add(a);
  }

	public void addCorpse(Corpse c) {
    corpses.add(c);
  }
    
  public void step() {
    //Aquí va la lógica de lo que hace el entorno, ahora sólo es activar a los agentes
    for(Agent a : agents) {
    	a.act(this);
    }
  }

	public double fracAdjacentCorpsesAt(int x, int y) {
		double n = 0;
		for(Corpse c : corpses) {
			boolean isInNeighborhood = false;
    	for(int i = -1; i <= 1; i++) {
				for(int j = -1; j <= 1; j++) {
					if(c.getX() == x + i && c.getY() == y + j) {
						n++;
						isInNeighborhood = true;
						break;
					}
				}
				if(isInNeighborhood) {
					break;
				}
			}
    }
		return n / 9;
	}

	public Corpse getCorpsetAt(int x, int y) {
		for (int i = 0; i < corpses.size(); i++) {
			Corpse c = corpses.get(i);
			if(c.getX() == x && c.getY() == y) {
				return c;
			}
		}
		return null;
	}
}
