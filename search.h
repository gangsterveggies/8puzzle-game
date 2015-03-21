#ifndef _SEARCH_
#define _SEARCH_

#include "common.h"

class Search
{
 public:

  virtual ~Search() {};

  virtual void solve(board initial_board, board final_board) {};
  virtual vector<Move> get_solution() {};

 private:

  virtual vector<Move> reconstruct_solution(node* final_node) {};
};

#endif
