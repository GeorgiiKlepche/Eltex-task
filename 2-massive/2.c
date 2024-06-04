#include <stdio.h>

#define N 5

int main() {
  int arr[N];

  printf("Введите массив из %d сиволов: ", N);
  for (int i = 0; i < N; i++) {
    scanf("%d", &arr[i]);
  }

  printf("Заданный массив: ");
  for (int i = 0; i < N; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // Меняем первый эллемент с последним и т.д
  int temp = 0, j = N - 1;
  for (int i = 0; i < (int)N / 2; i++) {
    temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
    j -= 1;
  }

  printf("Конечный массив: ");
  for (int i = 0; i < N; i++) {
    printf("%d ", arr[i]);
  }

  return 0;
}