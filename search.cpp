#include "search.h"

vector<Move> Search::reconstruct_solution(node* final_node)
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

int Search::get_used_nodes()
{
  return maximum_nodes;
}

void Search::add_node()
{
  used_nodes++;
  maximum_nodes = max(used_nodes, maximum_nodes);
}

void Search::free_node()
{
  used_nodes--;
}
