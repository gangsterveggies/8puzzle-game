#ifndef _SEARCH_
#define _SEARCH_

#include "common.h"

class Search
{
 public:

  virtual ~Search() {};

  virtual void solve(board initial_board, board final_board) {};
  virtual vector<Move> get_solution() {};
  int get_used_nodes();

 protected:

  int used_nodes;
  int maximum_nodes;

  vector<Move> reconstruct_solution(node* final_node);
  void add_node();
  void free_node();
};

#endif
