#ifndef _IDDFS_
#define _IDDFS_

#include "common.h"
#include "search.h"
#include "board.h"
#include <map>

class IDDFS : public Search
{
 public:

  IDDFS();
  ~IDDFS();

  void solve(board initial_board, board final_board);
  vector<Move> get_solution();

 private:

  map<board, int> visited; // Already visited moves
  board objetive_board; // Objetive board
  vector<Move> solution; // Solution obtained

  node* solve_recursive(node* current_node, int limit);
};

#endif
