#ifndef _COMMON_
#define _COMMON_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

typedef enum { Left, Right, Up, Down } Move;
typedef unsigned long long int board;

/*
 * Board representation:
 *
 *  1   2   3  ...  N
 * N+1 N+2 N+3 ... N+N
 * ... ... ... ... ...
 *
 * b_11b_12b_13...b_1kb_21b_22...b_lk
 *
 * l = NxN, total number of squares
 * k = log(NxN), mininum number of bits to encode a square
 *
 * b_i1...b_ik represents what square is on cell i
 *
 */

class Global
{
 public:
  static int N; // Board size NxN
  static int k; // Minimum bit representation of board
  static unsigned long long int bitmask; // Bitmask to extract squares from board
};

struct node
{
  node *parent;
  int depth;
  int cost;
  board current;
  Move last_move;
} typedef node;

typedef pair<int, node *> queue_node; // pair<priority, node>


#endif
