#ifndef _DFS_
#define _DFS_

#include "common.h"
#include "search.h"
#include "board.h"
#include <map>

class DFS : public Search
{
 public:

  DFS();
  ~DFS();

  void solve(board initial_board, board final_board);
  vector<Move> get_solution();

 private:

  map<board, int> visited; // Already visited moves
  board objetive_board; // Objetive board
  vector<Move> solution; // Solution obtained

  node* solve_recursive(node* current_node);
  vector<Move> reconstruct_solution(node* final_node);
};

#endif
