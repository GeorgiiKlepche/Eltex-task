#include <stdio.h>

void printBinary(int num) {
    if (num > 1) {
        printBinary(num >> 1);
    }
    printf("%d", num & 1);
}

int main() {
    int number;

    printf("Введите положительное целое число:");
    scanf("%d", &number);

    if (number < 0) {
        printf("Введите положительное число\n");
    } else {
        printf("Двоичное представление:");
        printBinary(number);
    }

    return 0;
}
