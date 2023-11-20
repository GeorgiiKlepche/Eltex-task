#include <stdio.h>

int main(void) {
  int num;
  printf("Введите первое число: ");
  scanf("%d", &num);

  int newByte;
  printf("Введите новый 3й байт: ");
  scanf("%d", &newByte);

  char *ptr = (char *)&num + 2;
  *(ptr) = newByte;
  printf("Новое число: %d", num);
}