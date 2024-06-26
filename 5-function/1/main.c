#include <stdio.h>
#include <string.h>

#define LENGTH 100

struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
};

void add_abonent(struct abonent users[], int *count) {
    if (*count < LENGTH) {
        printf("\nвведите имя: ");
        scanf("%10s", users[*count].name);
        printf("введите фамилию: ");
        scanf("%10s", users[*count].second_name);
        printf("введите телефон: ");
        scanf("%10s", users[*count].tel);
        (*count)++;
    } else {
        printf("Пользователей больше 100, справочник переполнен!\n");
    }
}

void delete_abonent(struct abonent users[], int *count) {
    char nameToDelete[10];
    printf("\nвведите имя для удаления: ");
    scanf("%10s", nameToDelete);
    int delete = 0;
    for (int i = 0; i < *count; i++) {
        if (strcmp(users[i].name, nameToDelete) == 0) {
            memset(&users[i], 0, sizeof(struct abonent));
            printf("абонент удален\n");
            delete = 1;
            break;
        }
    }

    if (!delete) {
        printf("\nабонент с именем %s не найден\n", nameToDelete);
    }
}

void search_abonent(struct abonent users[], int count) {
    char searchName[10];
    printf("\nвведите имя для поиска: ");
    scanf("%10s", searchName);
    int search = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].name, searchName) == 0) {
            search = 1;
            printf("имя: %s, фамилия: %s, телефон: %s\n", users[i].name,
                   users[i].second_name, users[i].tel);
        }
    }
    if (!search) {
        printf("\nабонент с именем %s не найден\n", searchName);
    }
}

void print_all_abonents(struct abonent users[], int count) {
    printf("\ncписок всех абонентов:\n");
    for (int i = 0; i < count; i++) {
        printf("имя: %s, фамилия: %s, телефон: %s\n", users[i].name,
               users[i].second_name, users[i].tel);
    }
}

int main() {
    struct abonent users[LENGTH];
    int input = -1, count = 0;

    while (input != 5) {
        printf("\n1) добавить абонента\n");
        printf("2) удалить абонента\n");
        printf("3) поиск абонетов по имени\n");
        printf("4) вывод всех записей\n");
        printf("5) выход\n");
        printf("Введите команду: ");
        scanf("%d", &input);

        switch (input) {
            case 1:
                add_abonent(users, &count);
                break;
            case 2:
                delete_abonent(users, &count);
                break;
            case 3:
                search_abonent(users, count);
                break;
            case 4:
                print_all_abonents(users, count);
                break;
            case 5:
                return 0;
                break;
            default:
                printf("\nНесуществующая команда\n\n");
                break;
        }
    }

    return 0;
}
