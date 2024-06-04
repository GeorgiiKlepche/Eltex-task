#include <stdio.h>

int count = 0;

int BinaryOnes(int num) {
    if (num > 1) {
        BinaryOnes(num >> 1);
    }
    if (num & 1 == 1) {
        count += 1;
    }
}

int main() {
    int number;

    printf("Введите положительное целое число:");
    scanf("%d", &number);

    if (number < 0) {
        printf("Введите положительное число\n");
    } else {
        BinaryOnes(number);
        printf("Количество едениц: %d ", count);
    }

    return 0;
}
