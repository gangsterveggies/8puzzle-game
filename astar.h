#ifndef _ASTAR_
#define _ASTAR_

#include "common.h"
#include "search.h"
#include "board.h"
#include <set>
#include <queue>

class AStar : public Search
{
 public:

  AStar();
  ~AStar();

  void solve(board initial_board, board final_board);
  vector<Move> get_solution();

 private:

  set<board> visited; // Already visited moves
  priority_queue<queue_node> state_queue; // States queue
  board objetive_board; // Objetive board
  vector<Move> solution; // Solution obtained
  vector<node*> state_list; // Found states to be deleted

  node* solve_astar(node* start_node);
};

#endif
