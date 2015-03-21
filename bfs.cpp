#include "bfs.h"

BFS::BFS()
{
}

BFS::~BFS()
{
  solution.clear();
  visited.clear();

  int i;
  for (i = 0; i < (int)state_list.size(); i++)
    delete state_list[i];

  state_list.clear();
}

vector<Move> BFS::reconstruct_solution(node* final_node)
{
  vector<Move> list_moves;

  if (final_node == NULL)
    return list_moves;
  
  node* current_node = final_node;
  while (current_node->parent != NULL)
  {
    list_moves.push_back(current_node->last_move);
    current_node = current_node->parent;
  }

  reverse(list_moves.begin(), list_moves.end());

  return list_moves;
}

node* BFS::solve_bfs(node* start_node)
{
  node* current_node;
  int i;

  state_queue.push(queue_node(0, start_node));

  while (1)
  {
    current_node = state_queue.front().second;
    state_queue.pop();

    if (current_node->current == objetive_board)
      return current_node;

    for (i = 0; i < Board::total_moves; i++)
    {
      board next_board = Board::move_square(current_node->current, Board::possible_moves[i]);

      if (Board::empty(next_board))
        continue;

      if (visited.find(next_board) != visited.end())
        continue;
      visited.insert(next_board);

      node* next_node = new node();
      next_node->current = next_board;
      next_node->parent = current_node;
      next_node->last_move = Board::possible_moves[i];
      next_node->depth = current_node->depth + 1;
      next_node->cost = current_node->cost + 1;
      state_list.push_back(next_node);

      state_queue.push(queue_node(next_node->cost, next_node));
    }
  }
}

void BFS::solve(board initial_board, board final_board)
{
  objetive_board = final_board;

  node* start_node = new node();
  start_node->current = initial_board;
  start_node->parent = NULL;
  start_node->last_move = Left; // No move to reach initial, so only filler move
  start_node->depth = 0;
  start_node->cost = 0;

  node* solution_node = solve_bfs(start_node);
  solution = reconstruct_solution(solution_node);

  delete start_node;
}

vector<Move> BFS::get_solution()
{
  return solution;
}
