#include "handbook.h"

void add_sub(char *name, char *second_name, char *tel, struct abonent **users,
             int *count) {
  *users = realloc(*users, (*count + 1) * sizeof(struct abonent));
  if (*users == NULL) {
    printf("Ошибка выделения памяти\n");
    exit(EXIT_FAILURE);
  }

  strcpy((*users)[*count].name, name);
  strcpy((*users)[*count].second_name, second_name);
  strcpy((*users)[*count].tel, tel);
  (*count)++;
}

void delete_sub(char *nameToDelete, struct abonent **users, int *count) {
  int delete = 0;
  for (int i = 0; i < *count; i++) {
    if (strcmp((*users)[i].name, nameToDelete) == 0) {
      if (i != *count - 1) {
        (*users)[i] = (*users)[*count - 1];
      }
      memset(&(*users)[*count - 1], 0, sizeof(struct abonent));
      delete = 1;
      printf("Абонент удален\n");
      break;
    }
  }

  if (!delete) {
    printf("Абонент с именем %s не найден\n", nameToDelete);
  } else {
    (*count)--;
    *users = realloc(*users, *count * sizeof(struct abonent));
    if (*count > 0 && *users == NULL) {
      printf("Ошибка освобождения памяти\n");
      exit(EXIT_FAILURE);
    }
  }
}

void search_sub(char *searchName, struct abonent *users, int count) {
  int search = 0;
  for (int i = 0; i < count; i++) {
    if (strcmp(users[i].name, searchName) == 0) {
      search = 1;
      printf("Имя: %s, Фамилия: %s, Телефон: %s\n", users[i].name,
             users[i].second_name, users[i].tel);
    }
  }
  if (!search) {
    printf("Абонент с именем %s не найден\n", searchName);
  }
}

void print_all_subs(struct abonent *users, int count) {
  printf("\nСписок всех абонентов:\n");
  for (int i = 0; i < count; i++) {
    printf("Имя: %s, Фамилия: %s, Телефон: %s\n", users[i].name,
           users[i].second_name, users[i].tel);
  }
}