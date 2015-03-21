#ifndef _IDASTAR_
#define _IDASTAR_

#include "common.h"
#include "search.h"
#include "board.h"
#include <map>

class IDAStar : public Search
{
 public:

  IDAStar();
  ~IDAStar();

  void solve(board initial_board, board final_board);
  vector<Move> get_solution();

 private:

  map<board, int> visited; // Already visited moves
  board objetive_board; // Objetive board
  vector<Move> solution; // Solution obtained

  queue_node solve_recursive(node* current_node, int limit);
};

#endif
