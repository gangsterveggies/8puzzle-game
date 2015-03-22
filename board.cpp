#include "board.h"

Move Board::possible_moves[] = { Left, Right, Up, Down };
int Board::total_moves = 4;
int Board::random_swaps = 237;
int Board::gods_number[] = {0, 0, 10, 31, 80};
int* Board::position_matrix;

void Board::set_board(board objetive_board)
{
  position_matrix = new int[Global::N * Global::N];

  int i, j;
  for (i = 0; i < Global::N; i++)
    for (j = 0; j < Global::N; j++)
      position_matrix[Global::N * i + j] = get_square(Global::N * i + j, objetive_board);
}

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

board Board::rotate_board(board input_board)
{
  int i, j;
  for (i = 0; i < Global::N - 1; i++)
    for (j = i + 1; j < Global::N; j++)
      input_board = swap_square(i * Global::N + j, j * Global::N + i, input_board);
  return input_board;
}

board Board::read_board(FILE* input_file)
{
  board input_board = 0;
  int i;

  if (input_file == stdin)
  {
    printf("Order:\n");

    int j;
    for (i = 0; i < Global::N; i++)
    {
      printf("\t");
      for (j = 0; j < Global::N; j++)
        printf("%d%c", i * Global::N + j, j == Global::N - 1 ? '\n' : ' ');
    }
  
    printf("(use 0 for empty square)\n");
  }

  int vl;
  for (i = 0; i < Global::N * Global::N; i++)
  {
    fscanf(input_file, "%d", &vl);
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

void Board::print_setup(board initial_board, board final_board)
{
  int double_digits = Global::N * Global::N - 1 >= 10;
  int i, j;
  for (i = 0; i < Global::N; i++)
  {
    if (i == Global::N / 2)
      printf("  From  ");
    else
      printf("        ");
    for (j = 0; j < Global::N; j++)
    {
      if (double_digits)
        printf("%2d ", get_square(i * Global::N + j, initial_board));
      else
        printf("%d ", get_square(i * Global::N + j, initial_board));
    }
      
    if (i == Global::N / 2)
      printf("  To   ");
    else
      printf("       ");

    for (j = 0; j < Global::N; j++)
    {
      if (double_digits)
        printf("%2d%c", get_square(i * Global::N + j, final_board), j == Global::N - 1 ? '\n' : ' ');
      else
        printf("%d%c", get_square(i * Global::N + j, final_board), j == Global::N - 1 ? '\n' : ' ');
    }
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

int Board::heuristic(board input_board, board objetive_board)
{
  int h1 = 0, h2 = 0, h3 = 0, h4 = 0, h5 = 0;
  int i, j, k;

  for (i = 0; i < Global::N; i++)
    for (j = 0; j < Global::N; j++)
    {
      int real_value = get_square(Global::N * i + j, input_board);
      int expected_value = get_square(Global::N * i + j, objetive_board);
      int real_i = real_value / Global::N;
      int real_j = real_value % Global::N;
      int expected_i = expected_value / Global::N;
      int expected_j = expected_value % Global::N;

      if (real_value != expected_value)
      {
        h1++;
        h2 += abs(position_matrix[real_value] / Global::N - i) + abs(position_matrix[real_value] % Global::N - j);
      }

      /*    for (k = 0; k < Global::N; k++)
      {
        int real_value_k = get_square(Global::N * i + k, input_board);
        int expected_value_k = get_square(Global::N * i + k, objetive_board);
        int real_k_j = real_value_k % Global::N;
        int expected_k_j = expected_value_k % Global::N;

        h3 += ((real_k_j < real_j) != (expected_k_j < expected_j));
        }*/
    }

/*  int** board_matrix = new int*[Global::N];
  int* position_matrix = new int[Global::N * Global::N];
  for (i = 0; i < Global::N; i++)
  {
    board_matrix[i] = new int[Global::N];
    for (j = 0; j < Global::N; j++)
    {
      board_matrix[i][j] = get_square(Global::N * i + j, input_board);
      position_matrix[Global::N * i + j] = board_matrix[i][j];
    }
  }

  for (i = 0; i < Global::N; i++)
    for (j = 0; j < Global::N; j++)
    {
      h5 += abs(position_matrix[board_matrix[i][j]] / Global::N - i);
      h5 += abs(position_matrix[board_matrix[i][j]] % Global::N - j);
    }

  for (i = 0; i < Global::N; i++)
    delete[] board_matrix[i];
  delete[] board_matrix;
  delete[] position_matrix;

  int* board_array = new int[Global::N * Global::N - 1];

  int current_index = 0;
  for (i = 0; i < Global::N * Global::N; i++)
  {
    int current_value = get_square(i, input_board);

    if (current_value)
      board_array[current_index++] = current_value;
  }

  int inversions = inversion_count(board_array, current_index);

  h4 = inversions / 3 + inversions % 3;

  input_board = rotate_board(input_board);

  current_index = 0;
  for (i = 0; i < Global::N * Global::N; i++)
  {
    int current_value = get_square(i, input_board);

    if (current_value)
      board_array[current_index++] = current_value;
  }

  inversions = inversion_count(board_array, current_index);
  delete[] board_array;

  h4 += inversions / 3 + inversions % 3;*/

  return max(h1, max(h2, max(h3, max(h4, h5))));
}

int Board::invariant(board input_board)
{
  int h1 = 0, h2 = 0;

  int i, j;

  for (i = 0; i < Global::N * Global::N; i++)
  {
    int current_value = get_square(i, input_board);
    if (current_value == 0 && Global::N % 2 == 0)
      h1 = i / Global::N + 1;

    if (current_value == 0)
      continue;

    for (j = 0; j < i; j++)
      if (get_square(j, input_board) > current_value)
        h2++;
  }

  return (h1 + h2) % 2;
}

int Board::solvable(board initial_board, board final_board)
{
  return invariant(initial_board) == invariant(final_board);
}

board Board::random_board()
{
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
  
int Board::inversion_count(int arr[], int n)
{
  int* temp = new int[n];
  int ans = merge_sort_count(arr, temp, 0, n - 1);
  delete[] temp;
  return ans;
}
  
int Board::merge_sort_count(int arr[], int temp[], int left, int right)
{
  int inv_count = 0;

  if (right > left)
  {
    int middle = (right + left) / 2;

    inv_count = merge_sort_count(arr, temp, left, middle);
    inv_count += merge_sort_count(arr, temp, middle + 1, right);
  
    inv_count += merge(arr, temp, left, middle + 1, right);
  }

  return inv_count;
}

int Board::merge(int arr[], int temp[], int left, int mid, int right)
{
  int i, j, k;
  int inv_count = 0;
  
  i = left;
  j = mid;
  k = left;

  while ((i <= mid - 1) && (j <= right))
  {
    if (arr[i] <= arr[j])
    {
      temp[k++] = arr[i++];
    }
    else
    {
      temp[k++] = arr[j++];
      inv_count = inv_count + (mid - i);
    }
  }

  while (i <= mid - 1)
    temp[k++] = arr[i++];

  while (j <= right)
    temp[k++] = arr[j++];

  for (i=left; i <= right; i++)
    arr[i] = temp[i];
  
  return inv_count;
}
