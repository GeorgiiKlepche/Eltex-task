#include <stdio.h>

#include "libcalc.h"

void print_menu() {
  printf("1) Сложение\n2) Вычитание\n3) Умножение\n4) Деление\n5) Выход\n");
  printf("Введите операцию: ");
}

int main() {
  int num1, num2, input;
  while (1) {
    print_menu();
    scanf("%d", &input);
    switch (input) {
      case 1:
        printf("\nВы выбрали операцию сложения\n");
        printf("Введите 2 целых числа: ");
        scanf("%d %d", &num1, &num2);
        printf("ОТВЕТ: %d + %d = %d\n\n", num1, num2, add(num1, num2));
        break;
      case 2:
        printf("Вы выбрали операцию вычитание\n");
        printf("Введите 2 целых числа: ");
        scanf("%d %d", &num1, &num2);
        printf("ОТВЕТ: %d - %d = %d\n\n", num1, num2, sub(num1, num2));
        break;
      case 3:
        printf("Вы выбрали операцию умножение\n");
        printf("Введите 2 целых числа: ");
        scanf("%d %d", &num1, &num2);
        printf("ОТВЕТ: %d * %d = %d\n\n", num1, num2, mul(num1, num2));
        break;
      case 4:
        printf("Вы выбрали операцию деление\n");
        printf("Введите 2 целых числа: ");
        scanf("%d %d", &num1, &num2);
        printf("ОТВЕТ: %d / %d = %d\n\n", num1, num2, div(num1, num2));
        break;
      case 5:
        return 0;

      default:
        printf("Неверная операция, повторите выбор\n");
        break;
    }
  }
}