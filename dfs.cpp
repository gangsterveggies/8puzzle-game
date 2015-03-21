#include "dfs.h"

DFS::DFS()
{
}

DFS::~DFS()
{
  solution.clear();
  visited.clear();
}

vector<Move> DFS::reconstruct_solution(node* final_node)
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

node* DFS::solve_recursive(node* current_node)
{ 
  if (current_node->current == objetive_board)
    return current_node;

  if (current_node->depth >= Board::gods_number[Global::N])
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
    
    node* next_node = new node();
    next_node->current = next_board;
    next_node->parent = current_node;
    next_node->last_move = Board::possible_moves[i];
    next_node->depth = current_node->depth + 1;
    next_node->cost = current_node->cost + 1;

    node* solution_node = solve_recursive(next_node);

    if (solution_node != NULL)
      return solution_node;

    delete next_node;
  }

  return NULL;
}

void DFS::solve(board initial_board, board final_board)
{
  objetive_board = final_board;

  node* start_node = new node();
  start_node->current = initial_board;
  start_node->parent = NULL;
  start_node->last_move = Left; // No move to reach initial, so only filler move
  start_node->depth = 0;
  start_node->cost = 0;

  node* solution_node = solve_recursive(start_node);
  solution = reconstruct_solution(solution_node);

  if (solution_node == NULL)
    return;

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

vector<Move> DFS::get_solution()
{
  return solution;
}
