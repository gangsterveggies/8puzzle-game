#include "board.h"

Move Board::possible_moves[] = { Left, Right, Up, Down };
int Board::total_moves = 4;
int Board::random_swaps = 137;

int Board::empty(board input_board)
{
  return input_board == 0;
}

int Board::get_square(int position, board input_board)
{
  return (int)((input_board >> (position * Global::k)) & Global::bitmask);
}

board Board::set_square(int position, int value, board input_board)
{
  return (input_board & ~(((unsigned long long int) Global::bitmask) << (position * Global::k))) | (((unsigned long long int) value) << (position * Global::k));
}

board Board::swap_square(int position_i, int position_j, board input_board)
{
  int value_i = get_square(position_i, input_board);
  int value_j = get_square(position_j, input_board);
  return set_square(position_j, value_i, set_square(position_i, value_j, input_board));
}

board Board::read_board()
{
  board input_board = 0;

  printf("Order:\n");

  int i, j;
  for (i = 0; i < Global::N; i++)
  {
    printf("\t");
    for (j = 0; j < Global::N; j++)
      printf("%d%c", i * Global::N + j, j == Global::N - 1 ? '\n' : ' ');
  }
  
  printf("(use 0 for empty square)\n");

  int vl;
  for (i = 0; i < Global::N * Global::N; i++)
  {
    scanf("%d", &vl);
    input_board = set_square(i, vl, input_board);
  }

  return input_board;
}

void Board::print_board(board input_board)
{
  int i, j;
  for (i = 0; i < Global::N; i++)
  {
    printf("\t");
    for (j = 0; j < Global::N; j++)
      printf("%d%c", get_square(i * Global::N + j, input_board), j == Global::N - 1 ? '\n' : ' ');
  }
}

board Board::move_square(board input_board, Move move)
{
  int i, zero_position;
  for (i = 0; i < Global::N * Global::N; i++)
    if (get_square(i, input_board) == 0)
    {
      zero_position = i;
      break;
    }

  if (move == Left)
  {
    if (zero_position % Global::N == 0)
      return 0;

    return swap_square(zero_position, zero_position - 1, input_board);
  }
  else if (move == Right)
  {
    if ((zero_position + 1) % Global::N == 0)
      return 0;

    return swap_square(zero_position, zero_position + 1, input_board);
  }
  else if (move == Up)
  {
    if (zero_position < Global::N)
      return 0;

    return swap_square(zero_position, zero_position - Global::N, input_board);
  }
  else if (move == Down)
  {
    if (zero_position >= Global::N * (Global::N - 1))
      return 0;

    return swap_square(zero_position, zero_position + Global::N, input_board);
  }
}

int Board::heuristic(board input_board)
{
  int h1 = 0, h2 = 0;
  int i, j;

  for (i = 0; i < Global::N; i++)
    for(j = 0; j < Global::N; j++)
    {
      int vl = get_square(Global::N * i + j, input_board);
      if (vl != Global::N * i + j && vl != 0)
      {
        h1++;
        h2 += abs(i - vl / Global::N) + abs(j - vl % Global::N);
      }
    }

  return max(h1, h2);
}

int Board::invariant(board input_board)
{
  int h1 = 0, h2 = 0;

  int i;
  for (i = 0; i < Global::N * Global::N; i++)
    if (get_square(i, input_board) == 0)
      h1 = i;

  int visited[Global::N * Global::N];
  memset(visited, 0, sizeof visited);

  for (i = 0; i < Global::N * Global::N; i++)
  {
    if (visited[i])
      continue;

    int current = get_square(i, input_board);
    while (current != i)
    {
      visited[current] = 1;
      current = get_square(current, input_board);
      h2++;
    }
  }

  return (h1 + h2) % 2;
}

int Board::solvable(board initial_board, board final_board)
{
  return invariant(initial_board) == invariant(final_board);
}

board Board::random_board()
{
  srand(time(NULL));
  int i;
  board new_board = 0;

  for (i = 0; i < Global::N * Global::N; i++)
    new_board = set_square(i, i, new_board);

  for (i = 0; i < random_swaps; i++)
  {
    int move = rand() % total_moves;
    board next_board = move_square(new_board, possible_moves[move]);

    if (next_board)
      new_board = next_board;
  }

  return new_board;
}
