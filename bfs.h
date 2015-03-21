#ifndef _BFS_
#define _BFS_

#include "common.h"
#include "search.h"
#include "board.h"
#include <set>
#include <queue>

class BFS : public Search
{
 public:

  BFS();
  ~BFS();

  void solve(board initial_board, board final_board);
  vector<Move> get_solution();

 private:

  set<board> visited; // Already visited moves
  queue<queue_node> state_queue; // States queue
  board objetive_board; // Objetive board
  vector<Move> solution; // Solution obtained
  vector<node*> state_list; // Found states to be deleted

  vector<Move> reconstruct_solution(node* final_node);
  node* solve_bfs(node* start_node);
};

#endif
