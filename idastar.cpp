#include "idastar.h"

IDAStar::IDAStar()
{
}

IDAStar::~IDAStar()
{
  solution.clear();
  visited.clear();
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

  pair<int, Move>* pre_moves = new pair<int, Move>[4];
  int i;
  for (i = 0; i < Board::total_moves; i++)
  {
    board next_board = Board::move_square(current_node->current, Board::possible_moves[i]);
    pre_moves[i] = pair<int, Move>(next_board != 0 ? Board::heuristic(next_board, objetive_board) : 150, Board::possible_moves[i]);
  }

  sort(pre_moves, pre_moves + Board::total_moves);

  for (i = 0; i < Board::total_moves; i++)
  {
    board next_board = Board::move_square(current_node->current, pre_moves[i].second);

    if (Board::empty(next_board))
      continue;

    if (visited.find(next_board) != visited.end())
      if (visited[next_board] <= current_node->depth + 1)
        continue;
    visited[next_board] = current_node->depth + 1;

    add_node();
    node* next_node = new node();
    next_node->current = next_board;
    next_node->parent = current_node;
    next_node->last_move = pre_moves[i].second;
    next_node->depth = current_node->depth + 1;
    next_node->cost = next_node->depth + pre_moves[i].first;

    queue_node solution_state = solve_recursive(next_node, limit);

    if (solution_state.second != NULL)
    {
      delete[] pre_moves;
      return solution_state;
    }

    free_node();
    next_cost = min(next_cost, solution_state.first);
    delete next_node;
  }

  delete[] pre_moves;

  return queue_node(next_cost, NULL);
}

void IDAStar::solve(board initial_board, board final_board)
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
