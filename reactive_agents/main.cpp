/**
 * @author Axel Alexis Montejo Lopez.
 * For subject: Introducción a la inteligencia artificial.
 * NOTE: This code only runs on POSIX-like operating systems.
 * */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include <unistd.h>

// ------------------------------ Interfaces ------------------------------
// ------------------------------------------------------------------------

class Agent {
public:
  virtual bool move() = 0;
  virtual void print() = 0;
};

// ------------------------------ Clases ------------------------------
// --------------------------------------------------------------------

// ------------------------- Reactive Agent ---------------------------
class ReactiveAgent: public Agent {
public:
  ReactiveAgent(std::vector<std::string> env, char skin, uint r, uint c);
  ~ReactiveAgent();
  virtual bool move() = 0;
  void print();
  bool top_blocked();
  bool bottom_blocked();
  bool right_blocked();
  bool left_blocked();
  bool top_right_blocked();
  bool top_left_blocked();
  bool bottom_right_blocked();
  bool bottom_left_blocked();
  void go_to_top();
  void go_to_bottom();
  void go_to_right();
  void go_to_left();
  bool go_anywhere();

protected:
  std::vector<std::string>            environment;
  char                                skin;
  uint                                row;
  uint                                col;
  uint                                last_row;
  uint                                last_col;
};

// --------------------- Reactive Agent Prod Sys -----------------------
class ReactiveAgentProdSys: public ReactiveAgent {
public:
  ReactiveAgentProdSys(std::vector<std::string> env, char skin, uint r, uint c);
  ~ReactiveAgentProdSys();
  bool move();
};

class ReactiveAgentTLU: public ReactiveAgent {
public:
  ReactiveAgentTLU(std::vector<std::string> env, char skin, uint r, uint c);
  ~ReactiveAgentTLU();
  bool move();

private:
  std::vector<bool> get_sense();
  bool tlu(std::vector<float> w, float t);
};

// --------------------------- Environment -----------------------------
class Environment {
public:
  // Public methods.
  Environment(WINDOW* w, std::vector<std::string> env, int r, int c);
  ~Environment();
  void print();
  void add_agent(Agent*);
  void run();

private:
  // Private attributes.
  std::vector<std::string>            environment;
  uint                                rows;
  uint                                cols;
  WINDOW*                             window;
  std::vector<Agent*>                 agents;

  // Private methods.
  bool screen_is_adequate();
};

int main() {
  srand(time(NULL));
  std::vector<std::string> env_map = {
    "************************************************************                    ",
    "*                                                          *                    ",
    "*                           ***                            *                    ",
    "*                           ***                            *                    ",
    "*                                                          *                    ",
    "*                                                          *                    ",
    "*                                                          *                    ",
    "*                                                          *                    ",
    "*                                                          *                    ",
    "*                                                          *                    ",
    "*                                                          *                    ",
    "*                                                          *********************",
    "*       ******************************                                         *",
    "*       ******************************                                         *",
    "*       *****                    *****                                         *",
    "*       *****                    *****                                         *",
    "*       *****                    *****                                         *",
    "*       *****                    *****                                         *",
    "*                                                          *********************",
    "*                                                          *                    ",
    "*                    ****                                  *                    ",
    "*                    ****                                  *                    ",
    "*                    ****                    ****          *                    ",
    "*                        ***********         ****          *                    ",
    "*                        *         *                       *                    ",
    "*                        *         *                       *                    ",
    "*                        *         *                       *                    ",
    "**************************         *************************                    ",
  };
  
  WINDOW* win;
  win = initscr();
  
  Environment env(win, env_map, env_map.size(), env_map[0].size());
  ReactiveAgentProdSys prodSysAgent(env_map, 'X', 3, 4);
  ReactiveAgentTLU tluAgent(env_map, '+', 10, 25);
  
  env.add_agent(&prodSysAgent);
  env.add_agent(&tluAgent);

  while(true) {
    env.run();
    usleep(250000);
  }
  endwin();
  return 0;
}

// ------------------------------ Class Implementations ------------------------------
// -----------------------------------------------------------------------------------

// ---------------------------------- ReactiveAgent ----------------------------------
ReactiveAgent::ReactiveAgent(std::vector<std::string> env, char skin, uint r, uint c) {
  this->environment = env;
  this->skin = skin;
  this->row = r;
  this->col = c;
  this->last_col = c;
  this->last_row = r;
}

ReactiveAgent::~ReactiveAgent() {
}

void ReactiveAgent::print() {
  mvprintw(this->last_row, this->last_col, " ");
  mvprintw(this->row, this->col, "%c", this->skin);
}

bool ReactiveAgent::top_blocked() {
  return this->environment[this->row - 1][this->col] != ' ';
}
bool ReactiveAgent::bottom_blocked() {
  return this->environment[this->row + 1][this->col] != ' ';
}
bool ReactiveAgent::right_blocked() {
  return this->environment[this->row][this->col + 1] != ' ';
}
bool ReactiveAgent::left_blocked() {
  return this->environment[this->row][this->col - 1] != ' ';
}
bool ReactiveAgent::top_right_blocked() {
  return this->environment[this->row - 1][this->col + 1] != ' ';
}
bool ReactiveAgent::top_left_blocked() {
  return this->environment[this->row - 1][this->col - 1] != ' ';
}
bool ReactiveAgent::bottom_right_blocked() {
  return this->environment[this->row + 1][this->col + 1] != ' ';
}
bool ReactiveAgent::bottom_left_blocked() {
  return this->environment[this->row + 1][this->col - 1] != ' ';
}

void ReactiveAgent::go_to_top() {
  //Go to top.
  this->last_row = this->row--;
  this->last_col = this->col;
}
void ReactiveAgent::go_to_bottom() {
  //Go to bottom.
  this->last_row = this->row++;
  this->last_col = this->col;
}
void ReactiveAgent::go_to_right() {
  //Go to right.
  this->last_row = this->row;
  this->last_col = this->col++;
}
void ReactiveAgent::go_to_left() {
  //Go to left.
  this->last_row = this->row;
  this->last_col = this->col--;
}

bool ReactiveAgent::go_anywhere() {
  if(this->top_blocked() && this->bottom_blocked() && this->left_blocked() && this->right_blocked()) {
    return false;
  }
  bool cant_go = true;
  while (cant_go) {
    switch (rand() % 4) {
    case 0:
      if(!this->top_blocked()) {
        this->go_to_top();
        cant_go = false;
      }
      break;
    case 1:
      if(!this->bottom_blocked()) {
        this->go_to_bottom();
        cant_go = false;
      }
      break;
    case 2:
      if(!this->right_blocked()) {
        this->go_to_right();
        cant_go = false;
      }
      break;
    case 3:
      if(!this->left_blocked()) {
        this->go_to_left();
        cant_go = false;
      }
      break;
    }
  }
  return true;
}

// ------------------------------ ReactiveAgentProdSys ------------------------------
ReactiveAgentProdSys::ReactiveAgentProdSys(std::vector<std::string> env, char skin, uint r, uint c):
                      ReactiveAgent(env, skin, r, c) {
}

ReactiveAgentProdSys::~ReactiveAgentProdSys() {
}

bool ReactiveAgentProdSys::move() {
  if(this->left_blocked() && !this->top_blocked()) {
    //Go to top.
    this->go_to_top();
    return true;
  } else if(this->top_blocked() && !this->right_blocked()) {
    //Go to right.
    this->go_to_right();
    return true;
  } else if(this->right_blocked() && !this->bottom_blocked()) {
    //Go to bottom.
    this->go_to_bottom();
    return true;
  } else if(this->bottom_blocked() && !this->left_blocked()) {
    //Go to left.
    this->go_to_left();
    return true;
  } else if(this->top_right_blocked() && !this->right_blocked()) {
    //Is a concave corner.
    //Go to right.
    this->go_to_right();
    return true;
  } else if(this->top_left_blocked() && !this->top_blocked()) {
    //Is a concave corner.
    //Go to top.
    this->go_to_top();
    return true;
  } else if(this->bottom_left_blocked() && !this->left_blocked()) {
    //Is a concave corner.
    //Go to left.
    this->go_to_left();
    return true;
  } else if(this->bottom_right_blocked() && !this->bottom_blocked()) {
    //Is a concave corner.
    //Go to bottom.
    this->go_to_bottom();
    return true;
  } else {
    //Go to right.
    return this->go_anywhere();
  }
}

// ---------------------------------- Environment ----------------------------------
Environment::Environment(WINDOW* w, std::vector<std::string> env, int r, int c) {
  this->window = w;
  this->environment = env;
  this->rows = r;
  this->cols = c;
}

Environment::~Environment(){ }

void Environment::print() {
  for(uint i = 0; i < rows; i++) {
    for(uint j = 0; j < cols; j++) {
      mvprintw(i, j, "%c", this->environment[i][j]);
    }
  }
}

void Environment::add_agent(Agent* new_agent) {
  this->agents.push_back(new_agent);
}

void Environment::run() {
  if(!this->screen_is_adequate()) {
    refresh();
    return;
  }

  this->print();
  for(uint i = 0; i < this->agents.size(); i++) {
    this->agents[i]->move();
    this->agents[i]->print();
  }

  refresh();
}

bool Environment::screen_is_adequate() {
  //Verify console dimentions.
  int dim_x , dim_y;
  getmaxyx(this->window , dim_y , dim_x ); // Get dimentions.

  if(dim_y < this->rows || dim_x < this->cols) {
    //clear();
    std::string message;
    message = "La dimension de la ventana es: %d de alto, %d de ancho. ";
    message += "Se requiere al menos %d de alto y %d de ancho.";
    mvprintw(0,0, message.c_str(), dim_y , dim_x, this->rows, this->cols);
    return false;
  }
  return true;
}

// ------------------------------ ReactiveAgentTLU ------------------------------
ReactiveAgentTLU::ReactiveAgentTLU(std::vector<std::string> env, char skin, uint r, uint c):
                  ReactiveAgent(env, skin, r, c) {
}

ReactiveAgentTLU::~ReactiveAgentTLU() {
}

bool ReactiveAgentTLU::move() {
  std::vector<float> w1 = {1, -3, -0.2, -0.2, -0.2, -0.2, -0.2, 5};
  std::vector<float> w2 = {-0.1, 5, 1, -3, -0.2, -0.2, -0.2, -0.2};
  std::vector<float> w3 = {-0.2, -0.2, -0.2, 5, 1, -3, -0.2, -0.2};
  std::vector<float> w4 = {-0.2, -0.2, -0.2, -0.2, -0.2, 5, 1, -3};

  std::vector<float> w5 = {-0.2, 1, 5, -3, -0.2, -0.2, -0.2, -0.2};
  std::vector<float> w6 = {5, -3, -0.2, -0.2, -0.2, -0.2, -0.2, 1};
  std::vector<float> w7 = {-0.2, -0.2, -0.2, -0.2, -0.2, 1, 5, -3};
  std::vector<float> w8 = {-0.2, -0.2, -0.2, 1, 5, -3, -0.2, -0.2};

  if(this->tlu(w1, 4)) {
    this->go_to_top();
    return true;
  } else if(this->tlu(w2, 4)) {
    this->go_to_right();
    return true;
  } else if(this->tlu(w3, 4)) {
    this->go_to_bottom();
    return true;
  } else if(this->tlu(w4, 4)) {
    this->go_to_left();
    return true;
  } else if(this->tlu(w5, 4)) {
    this->go_to_right();
    return true;
  } else if(this->tlu(w6, 4)) {
    this->go_to_top();
    return true;
  } else if(this->tlu(w7, 4)) {
    this->go_to_left();
    return true;
  } else if(this->tlu(w8, 4)) {
    this->go_to_bottom();
    return true;
  } else {
    return this->go_anywhere();
  }
}

std::vector<bool> ReactiveAgentTLU::get_sense() {
  std::vector<bool> sense;
  
  sense.push_back(this->top_left_blocked()); // S1
  sense.push_back(this->top_blocked()); // S2
  sense.push_back(this->top_right_blocked()); // S3
  sense.push_back(this->right_blocked()); // S4
  sense.push_back(this->bottom_right_blocked()); // S5
  sense.push_back(this->bottom_blocked()); // S6
  sense.push_back(this->bottom_left_blocked()); // S7
  sense.push_back(this->left_blocked()); // S8

  return sense;
}

bool ReactiveAgentTLU::tlu(std::vector<float> w, float t) {
  std::vector<bool> sense = this->get_sense();
  float res = 0;

  for(uint i = 0; i < sense.size(); i++) {
    res += sense[i] * w[i];
  }
  return res > t;
}

// ------------------------------ End of File ------------------------------
