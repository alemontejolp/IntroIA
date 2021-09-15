/**
 * @author Axel Alexis Montejo Lopez.
 * 
 * For subject: Introducción a la inteligencia artificial at Universidad del Caribe.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <stdlib.h>
#include <time.h>

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
  uint heuristic;
public:
  Node(Node* node, ullint l);
  Node(uint state[n_rows][n_cols], ullint l);
  ~Node();

  static void copy_state(uint dest[n_rows][n_cols], uint source[n_rows][n_cols]);

  uint get_state_at(uint row, uint col);
  bool equals(Node* node);
  std::vector<Node*> compute_next_states();
  bool where_is_void(uint& row, uint& col);
  bool where_is(uint val, uint& row, uint& col);
  bool top_blocked();
  bool bottom_blocked();
  bool right_blocked();
  bool left_blocked();
  void copy_from(Node* node);
  void swap_positions(uint row1, uint col1, uint row2, uint col2);
  void print_states();
  ullint get_level();
  void apply_manhattan_distance(Node* goal);
  uint get_heuristic();
};

class SearchTree {
private:
  Node* head;
  std::vector<Node*> visited_nodes;
public:
  SearchTree();
  SearchTree(uint state[n_rows][n_cols]);
  ~SearchTree();

  static void print_nodes_and_heuristic(std::vector<Node*> nodes);
  static void insert_by_heuristic(std::vector<Node*>& base, const std::vector<Node*>& new_nodes);
  static void insert_by_heuristic_and_level(std::vector<Node*>& base, const std::vector<Node*>& new_nodes);

  bool set_initial_state(uint state[n_rows][n_cols]);
  ullint get_n_visited_nodes();
  std::vector<Node*> get_nonvisted_nodes_from(std::vector<Node*> new_nodes);
  void insert_not_in_frontier_nodes(std::vector<Node*>& base, const std::vector<Node*>& new_nodes);
  std::vector<Node*> get_not_in_frontier_nodes(const std::vector<Node*>& base, const std::vector<Node*>& new_nodes);
  Node* bfs(Node goal);
  Node* dfs(Node goal);
  Node* heuristic_search(Node goal);
  Node* A_star(Node goal);
  Node* generate_state_from(Node init);
};

//------------------------- Main Program -------------------------

int main(int argc, char *argv[]) {
  uint initial_state[n_rows][n_cols];
  std::string method;
  if(argc >= 2) {
    method.assign(argv[1]);
  }

  if(method != "generate") {
    for(register uint i = 0; i < n_rows; i++) {
      for(register uint j = 0; j < n_cols; j++) {
        std::cin >> initial_state[i][j];
      }
    }
  }

  SearchTree search_tree(initial_state);
  Node initial_node(initial_state, 0);

  uint goal_state[n_rows][n_cols] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 0 }
  };

  Node goal_node(goal_state, 0);

  if(method == "generate") {
    Node* generated = search_tree.generate_state_from(goal_node);
    generated->print_states();
    return 0;
  }

  std::cout << "Matriz de entrada: \n";
  initial_node.print_states();

  Node* goal = 0;

  std::cout << "Empezando búsqueda. \n";
  if(method == "bfs") {
    std::cout << "Usando el método BFS.\n";
    goal = search_tree.bfs(goal_node);
  } else if(method == "dfs") {
    std::cout << "Usando el método DFS.\n";
    goal = search_tree.dfs(goal_node);
  } else if(method == "heuristic") {
    std::cout << "Usando el método por heuristica.\n";
    goal = search_tree.heuristic_search(goal_node);
  } else if(method == "a-star") {
    std::cout << "Usando el método A*.\n";
    goal = search_tree.A_star(goal_node);
  } else {
    std::cout << "Método no proporcionado o no reconocido: " << method << "\n";
    std::cout << "Este programa recibe como argumento de linea de comandos alguno de los siguientes:\n";
    std::cout << "bfs\n";
    std::cout << "dfs\n";
    std::cout << "heuristic\n";
    std::cout << "a-star\n";
    std::cout << "generate\n";
  }

  if(goal) {
    goal->print_states();
    std::cout << "Cantidad de pasos para llegar a este resultado: " << goal->get_level() << std::endl;
    std::cout << "Cantidad de nodos explorados: " << search_tree.get_n_visited_nodes() << std::endl;
  } else {
    std::cout << "No se pudo llegar al estado objetivo\n";
  }

  return 0;
}

//------------------------- Class: Node -------------------------
Node::Node(Node* node, ullint l) {
  this->copy_from(node);
  this->level = l;
  this->heuristic = 0;
}

Node::Node(uint state[n_rows][n_cols], ullint l) {
  Node::copy_state(this->state, state);
  this->level = l;
  this->heuristic = 0;
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
    this->print_states();
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

      uint a, b;
      if(!new_node->where_is_void(a, b)) {
        std::cout << "previous state:\n";
        this->print_states();
        std::cout << "post state:\n";
        new_node->print_states();
        std::cout << "move row: " << positions[i][0] << " move col: " << positions[i][1] << "\n";
        std::cout << "row: " << row << " col: " << col << "\n";
        exit(1);
      }
    }
  }

  return new_states;
}

bool Node::where_is_void(uint& row, uint& col) {
  return this->where_is(0, row, col);
}

bool Node::where_is(uint val, uint& row, uint& col) {
  for(register uint i = 0; i < n_rows; i++) {
    for(register uint j = 0; j < n_cols; j++) {
     if(this->state[i][j] == val) {
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

void Node::apply_manhattan_distance(Node* goal) {
  int row, col, sum = 0;
  uint r, c;
  for (int i = 0; i < n_rows; i++) {
    for(int j = 0; j < n_cols; j++) {
      this->where_is(goal->get_state_at(i, j), r, c);
      row = r;
      col = c;
      sum += abs(i - row) + abs(j - col);
    }
  }
  this->heuristic = sum;
}

uint Node::get_heuristic() {
  return this->heuristic;
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

ullint SearchTree::get_n_visited_nodes() {
  return this->visited_nodes.size();
}

std::vector<Node*> SearchTree::get_nonvisted_nodes_from(std::vector<Node*> new_nodes) {
  std::vector<Node*> selected;
  for(uint i = 0; i < new_nodes.size(); i++) {
    bool is_new_state = true;
    for(uint j = 0; j < this->visited_nodes.size(); j++) {
      if(new_nodes[i]->equals(this->visited_nodes[j])) {
        is_new_state = false;
        //delete new_nodes[i]; //Not tested yet. xd
        break;
      }
    }
    if(is_new_state) {
      selected.push_back(new_nodes[i]);
    }
  }
  return selected;
}

void SearchTree::insert_not_in_frontier_nodes(std::vector<Node*>& base, const std::vector<Node*>& new_nodes) {
  for(uint i = 0; i < new_nodes.size(); i++) {
    bool not_in_frontier = true;
    for(ullint j = 0; j < base.size(); j++) {
      if(new_nodes[i]->equals(base[j])) {
        not_in_frontier = false;
        break;
      }
    }
    if(not_in_frontier) {
      base.push_back(new_nodes[i]);
    }
  }
}

Node* SearchTree::bfs(Node goal) {
  if(!this->head) {
    return 0;
  }

  std::vector<Node*> pending_nodes;

  pending_nodes.push_back(this->head);

  while (!pending_nodes.empty()) {
    Node* curr_node = pending_nodes.front();
    if(goal.equals(curr_node)) {
      return curr_node;
    }

    pending_nodes.erase(pending_nodes.begin());
    std::vector<Node*> new_states = curr_node->compute_next_states();
    this->visited_nodes.push_back(curr_node);

    std::vector<Node*> nonvisited_nodes = this->get_nonvisted_nodes_from(new_states);
    SearchTree::insert_not_in_frontier_nodes(pending_nodes, nonvisited_nodes);
    //pending_nodes.insert(pending_nodes.end(), nonvisited_nodes.begin(), nonvisited_nodes.end());
  }
  
  return 0; //There is no way to reach the goal. :(
}

Node* SearchTree::dfs(Node goal)  {
  if(!this->head) {
    return 0;
  }

  std::vector<Node*> pending_nodes;

  pending_nodes.push_back(this->head);

  ullint i = 0;

  while (!pending_nodes.empty()) {
    Node* curr_node = pending_nodes.back();
    if(goal.equals(curr_node)) {
      return curr_node;
    }

    pending_nodes.pop_back();
    std::vector<Node*> new_states = curr_node->compute_next_states();
    this->visited_nodes.push_back(curr_node);

    std::vector<Node*> nonvisited_nodes = this->get_nonvisted_nodes_from(new_states);
    //SearchTree::insert_not_in_frontier_nodes(pending_nodes, nonvisited_nodes);
    pending_nodes.insert(pending_nodes.end(), nonvisited_nodes.begin(), nonvisited_nodes.end());
  }
  
  return 0; //There is no way to reach the goal. :(
}

Node* SearchTree::heuristic_search(Node goal) {
  if(!this->head) {
    return 0;
  }

  std::vector<Node*> pending_nodes;
  pending_nodes.push_back(this->head);
  this->head->apply_manhattan_distance(&goal);

  while (!pending_nodes.empty()) {
    Node* curr_node = pending_nodes.front();
    pending_nodes.erase(pending_nodes.begin());
    this->visited_nodes.push_back(curr_node);

    if(goal.equals(curr_node)) {
      return curr_node;
    }

    std::vector<Node*> new_nodes = curr_node->compute_next_states();

    for(uint i = 0; i < new_nodes.size(); i++) {
      new_nodes[i]->apply_manhattan_distance(&goal);
    }

    std::vector<Node*> nonvisited_nodes = this->get_nonvisted_nodes_from(new_nodes);
    std::vector<Node*> not_frontier = SearchTree::get_not_in_frontier_nodes(pending_nodes, nonvisited_nodes);
    SearchTree::insert_by_heuristic(pending_nodes, not_frontier);
  }

  return 0;
}

void SearchTree::insert_by_heuristic(std::vector<Node*>& base, const std::vector<Node*>& new_nodes) {
  int i = 0;

  if(base.empty()) {
    base.push_back(new_nodes[0]);
    i++;
  }

  for(i; i < new_nodes.size(); i++) {
    bool inserted = false;
    for(int j = 0; j < base.size(); j++) {
      if(new_nodes[i]->get_heuristic() < base[j]->get_heuristic()) {
        base.insert(base.begin() + j, new_nodes[i]);
        inserted = true;
        break;
      }
    }
    if(!inserted) {
      base.push_back(new_nodes[i]);
    }
  }
}

void SearchTree::print_nodes_and_heuristic(std::vector<Node*> nodes) {
  for(ullint i = 0; i < nodes.size() && i < 6; i++) {
    nodes[i]->print_states();
    std::cout << "Heuristica: " << nodes[i]->get_heuristic() << "\n"; 
  }
  std::cout << "-----------------------------------------------------\n"; 
}

std::vector<Node*> SearchTree::get_not_in_frontier_nodes(const std::vector<Node*>& base, const std::vector<Node*>& new_nodes) {
  std::vector<Node*> not_frontier;

  for(uint i = 0; i < new_nodes.size(); i++) {
    bool not_in_frontier = true;
    for(ullint j = 0; j < base.size(); j++) {
      if(new_nodes[i]->equals(base[j])) {
        not_in_frontier = false;
        break;
      }
    }
    if(not_in_frontier) {
      not_frontier.push_back(new_nodes[i]);
    }
  }

  return not_frontier;
}

Node* SearchTree::A_star(Node goal) {
  if(!this->head) {
    return 0;
  }

  std::vector<Node*> pending_nodes;
  pending_nodes.push_back(this->head);
  this->head->apply_manhattan_distance(&goal);

  while (!pending_nodes.empty()) {
    Node* curr_node = pending_nodes.front();
    pending_nodes.erase(pending_nodes.begin());
    this->visited_nodes.push_back(curr_node);

    if(goal.equals(curr_node)) {
      return curr_node;
    }

    std::vector<Node*> new_nodes = curr_node->compute_next_states();
    for(uint i = 0; i < new_nodes.size(); i++) {
      new_nodes[i]->apply_manhattan_distance(&goal);
    }

    std::vector<Node*> nonvisited_nodes = this->get_nonvisted_nodes_from(new_nodes);
    std::vector<Node*> not_frontier = SearchTree::get_not_in_frontier_nodes(pending_nodes, nonvisited_nodes);
    SearchTree::insert_by_heuristic_and_level(pending_nodes, not_frontier);
  }

  return 0;
}

void SearchTree::insert_by_heuristic_and_level(std::vector<Node*>& base, const std::vector<Node*>& new_nodes) {
  int i = 0;

  if(base.empty()) {
    if(new_nodes.empty()) {
      std::cout << "No hay nuevos nodos que agregar a la frontera.\n";
      return;
    }

    base.push_back(new_nodes[0]);
    i++;
  }

  for(i; i < new_nodes.size(); i++) {
    bool inserted = false;
    for(int j = 0; j < base.size(); j++) {
      ullint cost_new = new_nodes[i]->get_heuristic() + new_nodes[i]->get_level();
      ullint cost_base = base[j]->get_heuristic() + base[j]->get_level();
      if( cost_new < cost_base ) {
        base.insert(base.begin() + j, new_nodes[i]);
        inserted = true;
        break;
      }
    }
    if(!inserted) {
      base.push_back(new_nodes[i]);
    }
  }
}

Node* SearchTree::generate_state_from(Node init) {
  srand (time(NULL));
  int n = rand() % 200 + 1;

  Node* curr_node = &init;

  for(int i = 0; i < n; i++) {
    std::vector<Node*> next_nodes = curr_node->compute_next_states();
    std::vector<Node*> non_visited = this->get_nonvisted_nodes_from(next_nodes);
    
    if(non_visited.empty()) {
      return curr_node;
    }
    
    int indx = rand() % non_visited.size();
    this->visited_nodes.push_back(curr_node);
    curr_node = non_visited[indx];
  }

  return curr_node;
}
