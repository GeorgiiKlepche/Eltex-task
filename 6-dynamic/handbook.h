#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 10
#define MAX_TEL_LENGTH 10

struct abonent {
  char name[MAX_NAME_LENGTH];
  char second_name[MAX_NAME_LENGTH];
  char tel[MAX_TEL_LENGTH];
};

void add_sub(char *name, char *second_name, char *tel, struct abonent **users,
             int *count);
void delete_sub(char *nameToDelete, struct abonent **users, int *count);
void search_sub(char *searchName, struct abonent *users, int count);
void print_all_subs(struct abonent *users, int count);