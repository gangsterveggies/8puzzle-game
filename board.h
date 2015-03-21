#ifndef _BOARD_
#define _BOARD_

#include "common.h"

class Board
{
 public:

  static Move possible_moves[];
  static int total_moves;

  static int empty(board input_board);
  static int get_square(int position, board input_board);
  static board set_square(int position, int value, board input_board);
  static board swap_square(int position_i, int position_j, board input_board);
  static board read_board();
  static void print_board(board input_board);
  static board move_square(board input_board, Move move);
};

#endif
