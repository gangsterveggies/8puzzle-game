#include "iddfs.h"

IDDFS::IDDFS()
{
}

IDDFS::~IDDFS()
{
  solution.clear();
  visited.clear();
}

node* IDDFS::solve_recursive(node* current_node, int limit)
{ 
  if (current_node->current == objetive_board)
    return current_node;

  if (current_node->depth >= limit)
    return NULL;

  int i;
  for (i = 0; i < Board::total_moves; i++)
  {
    board next_board = Board::move_square(current_node->current, Board::possible_moves[i]);

    if (Board::empty(next_board))
      continue;

    if (visited.find(next_board) != visited.end())
      if (visited[next_board] <= current_node->cost + 1)
        continue;
    visited[next_board] = current_node->cost + 1;

    add_node();
    node* next_node = new node();
    next_node->current = next_board;
    next_node->parent = current_node;
    next_node->last_move = Board::possible_moves[i];
    next_node->depth = current_node->depth + 1;
    next_node->cost = current_node->cost + 1;

    node* solution_node = solve_recursive(next_node, limit);

    if (solution_node != NULL)
      return solution_node;

    free_node();
    delete next_node;
  }

  return NULL;
}

void IDDFS::solve(board initial_board, board final_board)
{
  objetive_board = final_board;

  add_node();
  node* start_node = new node();
  start_node->current = initial_board;
  start_node->parent = NULL;
  start_node->last_move = Left; // No move to reach initial, so only filler move
  start_node->depth = 0;
  start_node->cost = 0;

  node* solution_node = NULL;
  int maximum_depth = 0;

  while (solution_node == NULL)
  {
    visited.clear();
    solution_node = solve_recursive(start_node, maximum_depth++);

    if (maximum_depth == Board::gods_number[Global::N])
      break;
  }

    if (solution_node == NULL)
    return;

  solution = reconstruct_solution(solution_node);

  node* current_node = solution_node;
  node* next_node;
  while (current_node->parent != NULL)
  {
    next_node = current_node->parent;
    delete current_node;
    current_node = next_node;
  }

  delete current_node;
}

vector<Move> IDDFS::get_solution()
{
  return solution;
}
