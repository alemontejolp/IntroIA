/**
 * @author Axel Alexis Montejo Lopez.
 * 
 * For subject: Introducción a la inteligencia artificial at Universidad del Caribe.
 */

#include <iostream>
#include <vector>
#include <queue>

//------------------------- Type declarations -------------------------

typedef unsigned int uint;
typedef unsigned long long int ullint;

//------------------------- Constant declarations -------------------------

const uint n_rows = 3;
const uint n_cols = 3;

//------------------------- Class declarations -------------------------

class Node {
private:
  uint state[n_rows][n_cols];
  ullint level;
public:
  std::vector<Node*> children;

  Node(Node* node, ullint l);
  Node(uint state[n_rows][n_cols], ullint l);
  ~Node();

  static void copy_state(uint dest[n_rows][n_cols], uint source[n_rows][n_cols]);

  uint get_state_at(uint row, uint col);
  bool equals(Node* node);
  std::vector<Node*> compute_next_states();
  bool where_is_void(uint& row, uint& col);
  bool top_blocked();
  bool bottom_blocked();
  bool right_blocked();
  bool left_blocked();
  void copy_from(Node* node);
  void swap_positions(uint row1, uint col1, uint row2, uint col2);
  void print_states();
  ullint get_level();
};

class SearchTree {
private:
  Node* head;
public:
  SearchTree();
  SearchTree(uint state[n_rows][n_cols]);
  ~SearchTree();

  bool set_initial_state(uint state[n_rows][n_cols]);
  Node* bfs(Node goal);
};

//------------------------- Main Program -------------------------

int main() {
  uint initial_state[n_rows][n_cols];

  for(register uint i = 0; i < n_rows; i++) {
    for(register uint j = 0; j < n_cols; j++) {
      std::cin >> initial_state[i][j];
    }
  }

  SearchTree search_tree(initial_state);

  //Fix the goal state.
  uint goal_state[n_rows][n_cols] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 0 }
  };

  Node goal_node(goal_state, 0);

  std::cout << "Empezando búsqueda. \n";

  Node* goal = search_tree.bfs(goal_node);
  if(goal) {
    goal->print_states();
    std::cout << "Cantidad de pasos para llegar a este resultado: " << goal->get_level() << std::endl;
  } else {
    std::cout << "No se pudo llegar al estado objetivo\n";
  }

  return 0;
}

//------------------------- Class: Node -------------------------
Node::Node(Node* node, ullint l) {
  this->copy_from(node);
  this->level = l;
}

Node::Node(uint state[n_rows][n_cols], ullint l) {
  Node::copy_state(this->state, state);
  this->level = l;
}

Node::~Node() {
}

void Node::copy_state(uint dest[n_rows][n_cols], uint source[n_rows][n_cols]) {
  for(register uint i = 0; i < n_rows; i++) {
    for(register uint j = 0; j < n_cols; j++) {
      dest[i][j] = source[i][j];
    }
  }
}

uint Node::get_state_at(uint row, uint col) {
  return this->state[row][col];
}

bool Node::equals(Node* node) {
  for(register uint i = 0; i < n_rows; i++) {
    for(register uint j = 0; j < n_cols; j++) {
      if(this->state[i][j] != node->get_state_at(i, j)) {
        return false;
      }
    }
  }

  return true;
}

std::vector<Node*> Node::compute_next_states() {
  std::vector<Node*> new_states;
  uint row, col;
  if(!this->where_is_void(row, col)) {
    std::cout << "No hay un espacio vacío en la matriz." << std::endl;
    return new_states;
  }

  Node* new_node = 0;

  bool available_directions[] = {
    !this->top_blocked(),
    !this->bottom_blocked(),
    !this->left_blocked(),
    !this->right_blocked()
  };

  int positions[][2] = {
    { -1, 0 },
    { 1, 0 },
    { 0, -1 },
    { 0, 1 }
  };

  for(uint i = 0; i < 4; i++) {
    if(available_directions[i]) {
      new_node = new Node(this, this->level + 1);
      new_node->swap_positions(row, col, row + positions[i][0], col + positions[i][1]);
      new_states.push_back(new_node);
    }
  }

  return new_states;
}

bool Node::where_is_void(uint& row, uint& col) {
  for(register uint i = 0; i < n_rows; i++) {
    for(register uint j = 0; j < n_cols; j++) {
     if(this->state[i][j] == 0) {
       row = i;
       col = j;
       return true;
     }
    }
  }
  return false;
}

bool Node::top_blocked() {
  uint row, col;
  this->where_is_void(row, col);
  return row == 0;
}

bool Node::bottom_blocked() {
  uint row, col;
  this->where_is_void(row, col);
  return row == n_rows - 1;
}

bool Node::right_blocked() {
  uint row, col;
  this->where_is_void(row, col);
  return col == n_cols - 1;
}

bool Node::left_blocked() {
  uint row, col;
  this->where_is_void(row, col);
  return col == 0;
}

void Node::swap_positions(uint row1, uint col1, uint row2, uint col2) {
  uint tmp = this->state[row1][col1];
  this->state[row1][col1] = this->state[row2][col2];
  this->state[row2][col2] = tmp;
}

void Node::copy_from(Node* node) {
  for(register uint i = 0; i < n_rows; i++) {
    for(register uint j = 0; j < n_cols; j++) {
      this->state[i][j] = node->get_state_at(i, j);
    }
  }
}

void Node::print_states() {
  for(register uint i = 0; i < n_rows; i++) {
    for(register uint j = 0; j < n_cols; j++) {
      std::cout << this->state[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

ullint Node::get_level() {
  return this->level;
}

//------------------------- Class: SearchTree -------------------------
SearchTree::SearchTree() {
  this->head = 0;
}

SearchTree::SearchTree(uint state[n_rows][n_cols]) {
  this->head = 0;
  this->set_initial_state(state);
}

SearchTree::~SearchTree() {
}

bool SearchTree::set_initial_state(uint state[n_rows][n_cols]) {
  if(this->head) {
    return false;
  }
  this->head = new Node(state, 0);
  return true;
}

Node* SearchTree::bfs(Node goal) {
  if(!this->head) {
    return 0;
  }

  std::queue<Node*> pending_nodes;
  std::vector<Node*> visited_nodes;

  pending_nodes.push(this->head);

  while (!pending_nodes.empty()) {
    Node* curr_node = pending_nodes.front();
    if(goal.equals(curr_node)) {
      return curr_node;
    }

    pending_nodes.pop();
    std::vector<Node*> new_states = curr_node->compute_next_states();
    visited_nodes.push_back(curr_node);

    for(uint i = 0; i < new_states.size(); i++) {
      bool is_new_state = true;
      for(uint j = 0; j < visited_nodes.size(); j++) {
        if(new_states[i]->equals(visited_nodes[j])) {
          is_new_state = false;
          break;
        }
      }
      if(is_new_state) {
        pending_nodes.push(new_states[i]);
      }
    }
  }
  
  return 0; //There is no way to reach the goal. :(
}
