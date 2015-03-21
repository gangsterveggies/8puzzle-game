#include "idastar.h"

IDAStar::IDAStar()
{
}

IDAStar::~IDAStar()
{
  solution.clear();
  visited.clear();
}

vector<Move> IDAStar::reconstruct_solution(node* final_node)
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

queue_node IDAStar::solve_recursive(node* current_node, int limit)
{
  int next_cost = 150;

  if (current_node->cost > limit)
    return queue_node(current_node->cost, NULL);

  if (current_node->depth >= Board::gods_number[Global::N])
    return queue_node(current_node->cost, NULL);

  if (current_node->current == objetive_board)
    return queue_node(limit, current_node);

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
    next_node->cost = next_node->depth + Board::heuristic(next_board, objetive_board);

    queue_node solution_state = solve_recursive(next_node, limit);

    if (solution_state.second != NULL)
      return solution_state;

    next_cost = min(next_cost, solution_state.first);
    delete next_node;
  }

  return queue_node(next_cost, NULL);
}

void IDAStar::solve(board initial_board, board final_board)
{
  objetive_board = final_board;

  node* start_node = new node();
  start_node->current = initial_board;
  start_node->parent = NULL;
  start_node->last_move = Left; // No move to reach initial, so only filler move
  start_node->depth = 0;
  start_node->cost = 0;

  node* solution_node = NULL;
  queue_node solution_state = queue_node(Board::heuristic(initial_board, final_board), NULL);
  int maximum_depth = 0;

  while (solution_state.second == NULL)
  {
    visited.clear();
    solution_state = solve_recursive(start_node, solution_state.first);

    if (solution_state.first >= 150)
      break;
  }

  solution_node = solution_state.second;

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

vector<Move> IDAStar::get_solution()
{
  return solution;
}
