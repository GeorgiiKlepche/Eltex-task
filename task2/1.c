#include <stdio.h>
#define N 3

int main() {
  int matrix[N][N];
  int cell = 1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = cell;
      cell += 1;
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }

  return 0;
}