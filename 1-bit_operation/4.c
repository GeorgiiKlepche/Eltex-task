#include <stdio.h>

int main() {
    int number, newValue, result;

    printf("Введите положительное целое число: ");
    scanf("%d", &number);

    if (number < 0) {
        printf("Введите положительное число\n");
        return 1;
    }

    printf("Введите третий бит (0 или 1): ");
    scanf("%d", &newValue);

    result = number & ~(1 << 2);
    result = result | (newValue << 2);

    printf("Новое число: %d ", result);

    return 0;
}
