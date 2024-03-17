#include "handbook.h"

int main() {
  int input = -1, count = 0;
  struct abonent *users = NULL;
  char name[MAX_NAME_LENGTH], second_name[MAX_NAME_LENGTH], tel[MAX_TEL_LENGTH];

  while (input != 5) {
    printf("\n1) Добавить абонента\n");
    printf("2) Удалить абонента\n");
    printf("3) Поиск абонетов по имени\n");
    printf("4) Вывод всех записей\n");
    printf("5) Выход\n");
    printf("Введите команду: ");
    scanf("%d", &input);
    switch (input) {
      case 1:
        printf("\nВведите имя: ");
        scanf("%s", name);
        printf("Введите фамилию: ");
        scanf("%s", second_name);
        printf("Введите телефон: ");
        scanf("%s", tel);
        add_sub(name, second_name, tel, &users, &count);
        break;
      case 2:
        printf("\nВведите имя для удаления: ");
        scanf("%s", name);
        delete_sub(name, &users, &count);
        break;
      case 3:
        printf("\nВведите имя для поиска: ");
        scanf("%s", name);
        search_sub(name, users, count);
        break;
      case 4:
        print_all_subs(users, count);
        break;
      case 5:
        free(users);
        return 0;
      default:
        printf("\nНесуществующая команда\n\n");
        break;
    }
  }
  return 0;
}