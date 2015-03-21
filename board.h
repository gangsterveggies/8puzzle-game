#ifndef _BOARD_
#define _BOARD_

#include "common.h"
#include <time.h>

class Board
{
 public:

  static Move possible_moves[];
  static int total_moves;
  static int random_swaps;
  static int gods_number[];

  static int empty(board input_board);
  static int get_square(int position, board input_board);
  static board set_square(int position, int value, board input_board);
  static board swap_square(int position_i, int position_j, board input_board);
  static board rotate_board(board input_board);
  static board read_board(FILE* input_file);
  static void print_board(board input_board);
  static void print_setup(board initial_board, board final_board);
  static board move_square(board input_board, Move move);
  static int heuristic(board input_board, board objetive_board);
  static int invariant(board input_board);
  static int solvable(board initial_board, board final_board);
  static board random_board();

 private:

  static int inversion_count(int arr[], int n);
  static int merge_sort_count(int arr[], int temp[], int left, int right);
  static int merge(int arr[], int temp[], int left, int mid, int right);
};

#endif
