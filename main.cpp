#include "common.h"
#include "board.h"
#include "search.h"
#include "dfs.h"
#include "bfs.h"
#include "iddfs.h"
#include "greedy.h"
#include "astar.h"
#include "idastar.h"
#include "timer.h"

#include <sys/types.h>
#include <unistd.h>

int Global::N; // Board size
int Global::k; // Minimum bit representation of board
unsigned long long int Global::bitmask; // Bitmask to extract squares from board
FILE* initial_file;
FILE* final_file;
int help_flag; // Show help info
int method_flag; // Search method to use
int verbose; // Print intermediate solution boards
int initial_randomize, final_randomize; // Generate random start/end boards
board initial_board, final_board; // Objetive boards
int maximum_nodes; // Maximum used nodes by search

void init()
{
  srand(time(NULL));

  initial_file = stdin;
  final_file = stdin;

  help_flag = 0;
  verbose = 0;
  initial_randomize = final_randomize = 0;

  Global::N = 3;
  Global::k = log2(Global::N * Global::N - 1) + 1;

  int j, vl = 0;
  for (j = 0; j < Global::k; j++)
  {
    vl <<= 1;
    vl |= 1;
  }

  Global::bitmask = vl;
  final_board = 23207117601;

  printf(" _______   _______     ___   .___________. __    __         _    \n"
         "|       \\ |   ____|   /   \\  |           ||  |  |  |     /\\| |/\\ \n"
         "|  .--.  ||  |__     /  ^  \\ `---|  |----`|  |__|  |     \\ ` ' / \n"
         "|  |  |  ||   __|   /  /_\\  \\    |  |     |   __   |    |_     _|\n"
         "|  '--'  ||  |____ /  _____  \\   |  |     |  |  |  |     / , . \\ \n"
         "|_______/ |_______/__/     \\__\\  |__|     |__|  |__|     \\/|_|\\/ \n"
         "\n      By Filipe Figueiredo, Pedro Paredes, Tiago Castanheira\n"
         "    ----------------------------------------------------------\n\n"
    );
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
    else if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--verbose") == 0))
      verbose = 1;
    else if ((strcmp(argv[i], "-m") == 0))
    {
      method_flag = max(5, min(0, atoi(argv[i + 1])));
      i++;
    }
    else if ((strcmp(argv[i], "-i") == 0))
    {
      initial_file = fopen(argv[i + 1], "r");
      i++;
    }
    else if ((strcmp(argv[i], "-f") == 0))
    {
      final_file = fopen(argv[i + 1], "r");
      i++;
    }
    else if (strcmp(argv[i], "-ir") == 0)
      initial_randomize = 1;
    else if (strcmp(argv[i], "-fr") == 0)
      final_randomize = 1;
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
    else if (strcmp(argv[i], "--idastar") == 0)
      method_flag = 5;
    else
      return 1;
  return 0;
}

void output_help()
{
  printf("\t\t---- N-puzzle Solver ----\n");
  printf("usage:\n");
  printf("\t./PUZZLE [arguments]\n\n");
  printf("Available arguments:\n");
  printf("\t-h, --help\t\tdisplay this help file\n"
         "\t-v, --verbose\t\tdisplay intermediate board positions on output\n"
         "\t-n <number>\t\tset puzzle size to <number>\n"
         "\t-m <method>\t\tset used method to <method> (integer from 0 to 5)\n"
         "\t-ir\t\t\tgenerate random initial board\n"
         "\t-fr\t\t\tgenerate random final board\n"
         "\t--dfs\t\t\tset used method to DFS\n"
         "\t--bfs\t\t\tset used method to BFS\n"
         "\t--iddfs\t\t\tset used method to IDDFS\n"
         "\t--greedy\t\tset used method to Greedy\n"
         "\t--astar\t\t\tset used method to A*\n"
         "\t--idastar\t\tset used method to IDA*\n\n"
    );
  printf("Note: default final board is: (it is overwritten when using -n)\n");
  Board::print_board(23207117601);
}

void read_input()
{
  if (initial_board == 0 || initial_randomize)
  {
    if (initial_randomize)
      initial_board = Board::random_board();
    else
    {
      if (initial_file == stdin)
        printf("Input initial board:\n");
      initial_board = Board::read_board(initial_file);
    }
  }

  if (final_board == 0 || final_randomize)
  {
    if (final_randomize)
      final_board = Board::random_board();
    else
    {
      if (final_file == stdin)
        printf("Input final board:\n");
      final_board = Board::read_board(final_file);
    }
  }
}

vector<Move> calculate()
{
  printf("Calculating:\n");
  Board::print_setup(initial_board, final_board);
  printf("\n");

  if (!Board::solvable(initial_board, final_board))
  {
    printf("No solution exists...\n");
    vector<Move> list_moves;
    return list_moves;
  }

  Board::set_board(final_board);

  Search* engine;
  if (method_flag == 0)
    engine = new DFS();
  else if (method_flag == 1)
    engine = new BFS();
  else if (method_flag == 2)
    engine = new IDDFS();
  else if (method_flag == 3)
    engine = new Greedy();
  else if (method_flag == 4)
    engine = new AStar();
  else if (method_flag == 5)
    engine = new IDAStar();

  engine->solve(initial_board, final_board);
  vector<Move> list_moves = engine->get_solution();
  maximum_nodes = engine->get_used_nodes();

  delete engine;

  return list_moves;
}

void output_result(vector<Move> list_moves)
{
  printf("Results output:\n");
  printf("\tNumber of steps: %d\n", (int)list_moves.size());

  if (!list_moves.empty())
  {
    printf("\tList of moves:\n");
    if (!verbose)
      printf("\t\t");
  }

  board current_board = initial_board;
  int i;
  for (i = 0; i < (int)list_moves.size(); i++)
  {
    if (list_moves[i] == Left)
      printf("Left");
    else if (list_moves[i] == Right)
      printf("Right");
    else if (list_moves[i] == Up)
      printf("Up");
    else if (list_moves[i] == Down)
      printf("Down");

    if (verbose)
    {
      printf("\n");
      current_board = Board::move_square(current_board, list_moves[i]);
      Board::print_board(current_board);
      printf("\n");
    }
    else
      printf("%s", i == (int)list_moves.size() - 1 ? "\n\n" : " | ");
  }

  printf("\tElapsed time: %lf\n", Timer::elapsed());
  printf("\tMaximum used nodes: %d\n", maximum_nodes);
  printf("\tEstimated maximum used (in search) memory: %d bytes\n", maximum_nodes * sizeof(node));
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

  Timer::start();
  vector<Move> list_moves = calculate();
  Timer::stop();

  output_result(list_moves);

  fclose(initial_file);
  fclose(final_file);

  return 0;
}
