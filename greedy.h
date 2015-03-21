#ifndef _GREEDY_
#define _GREEDY_

#include "common.h"
#include "search.h"
#include "board.h"
#include <set>
#include <queue>

class Greedy : public Search
{
 public:

  Greedy();
  ~Greedy();

  void solve(board initial_board, board final_board);
  vector<Move> get_solution();

 private:

  set<board> visited; // Already visited moves
  priority_queue<queue_node> state_queue; // States queue
  board objetive_board; // Objetive board
  vector<Move> solution; // Solution obtained
  vector<node*> state_list; // Found states to be deleted

  vector<Move> reconstruct_solution(node* final_node);
  node* solve_greedy(node* start_node);
};

#endif
