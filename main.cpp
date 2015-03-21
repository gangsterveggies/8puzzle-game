#include "common.h"
#include "board.h"
#include "search.h"
#include "dfs.h"

int Global::N; // Board size
int Global::k; // Minimum bit representation of board
unsigned long long int Global::bitmask; // Bitmask to extract squares from board
int help_flag; // Show help info
int method_flag; // Search method to use
board initial_board, final_board; // Objetive boards

void init()
{
  help_flag = 0;
  Global::N = 3;
  Global::k = log2(Global::N * Global::N - 1) + 1;

  int j, vl = 0;
  for (j = 0; j < Global::k; j++)
  {
    vl <<= 1;
    vl |= 1;
  }

  Global::bitmask = vl;
  initial_board = 23207117601;
}

int process_arguments(int argc, char *argv[])
{
  int i;
  for (i = 1; i < argc; i++)
    if (strcmp(argv[i], "-n") == 0 && i != argc - 1)
    {
      Global::N = atoi(argv[i + 1]);
      Global::k = log2(Global::N * Global::N - 1) + 1;

      int j, vl = 0;
      for (j = 0; j < Global::k; j++)
      {
        vl <<= 1;
        vl |= 1;
      }

      Global::bitmask = vl;
      
      initial_board = final_board = 0;
      i++;
    }
    else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
      help_flag = 1;
    else if ((strcmp(argv[i], "-m") == 0))
    {
      method_flag = max(4, min(0, atoi(argv[i + 1])));
      i++;
    }
    else if (strcmp(argv[i], "--dfs") == 0)
      method_flag = 0;
    else if (strcmp(argv[i], "--bfs") == 0)
      method_flag = 1;
    else if (strcmp(argv[i], "--iddfs") == 0)
      method_flag = 2;
    else if (strcmp(argv[i], "--greedy") == 0)
      method_flag = 3;
    else if (strcmp(argv[i], "--astar") == 0)
      method_flag = 4;
    else
      return 1;
  return 0;
}

void output_help()
{
  printf("\t\t---- N-puzzle Solver ----\n");
  printf("usage:\n");
  printf("\t./PUZZLE [-n <board_size>] [-h]\n");
}

void read_input()
{
  if (initial_board == 0)
  {
    printf("Input initial board:\n");
    initial_board = Board::read_board();
  }

  if (final_board == 0)
  {
    printf("Input final board:\n");
    final_board = Board::read_board();
  }
}

vector<Move> calculate()
{
  printf("Calculating from:\n");
  Board::print_board(initial_board);
  printf("to:\n");
  Board::print_board(final_board);
  printf("\n");

  Search* engine;
  if (method_flag == 0)
    engine = new DFS();
  else if (method_flag == 1)
    method_flag = 1;
  else if (method_flag == 2)
    method_flag = 2;
  else if (method_flag == 3)
    method_flag = 3;
  else if (method_flag == 4)
    method_flag = 4;

  engine->solve(initial_board, final_board);
  vector<Move> list_moves = engine->get_solution();
  delete engine;

  return list_moves;
}

void output_result(vector<Move> list_moves)
{
  printf("Number of steps: %d\n", (int)list_moves.size());
}

int main(int argc, char *argv[])
{
  init();

  if (process_arguments(argc, argv) || help_flag)
  {
    output_help();
    return !help_flag;
  }
  
  read_input();

  vector<Move> list_moves = calculate();

  output_result(list_moves);

  return 0;
}
