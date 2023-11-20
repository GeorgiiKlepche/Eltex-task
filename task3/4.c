#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 100

int main() {
    char str1[MAX_LENGTH];
    char str2[MAX_LENGTH];

    printf("Введите строку: ");
    scanf("%99s", str1);
    printf("Адрес введенной строки: %p\n", (void*)&str1);

    printf("Введите подстроку для поиска: ");
    scanf("%99s", str2);

    int len = strlen(str2);
    int str_len = strlen(str1);

    for (int i = 0; i <= str_len - len; i++) {
        int j;
        for (j = 0; j < len; j++) {
            if (str1[i + j] != str2[j]) {
                break;
            }
        }

        if (j == len) {
            printf("Подстрока найдена по адресу: %p\n", (void*)&str1[i]);
            return 0;
        }
    }

    printf("Подстрока не найдена\n");

    return 0;
}
