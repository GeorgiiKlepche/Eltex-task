#include <stdio.h>

#define N 5

int main() {
  int matrix[N][N];
  int top = 0, left = 0;
  int bot = N - 1, right = N - 1;
  int cell = 0;
  while (top <= bot && left <= right) {
    for (int j = left; j <= right; j++) {
      cell++;
      matrix[top][j] = cell;
    }
    top++;

    for (int i = top; i <= bot; i++) {
      cell++;
      matrix[i][right] = cell;
    }
    right--;

    for (int j = right; j >= left; j--) {
      cell++;
      matrix[bot][j] = cell;
    }
    bot--;

    for (int i = bot; i >= top; i--) {
      cell++;
      matrix[i][left] = cell;
    }
    left++;
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%2d ", matrix[i][j]);
    }
    printf("\n");
  }

  return 0;
}