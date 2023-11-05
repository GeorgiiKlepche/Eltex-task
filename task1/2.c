#include <stdio.h>

void printBinary(int num) {
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }
}

int main() {
    int number;

    printf("Введите целое отрицательное число: ");
    scanf("%d", &number);

    printf("Двоичное представление числа %d: ", number);
    printBinary(number);

    return 0;
}
