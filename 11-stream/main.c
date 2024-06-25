#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_STORES 5
#define NUM_BUYERS 3
#define MAX_DEMAND 110000
#define MIN_DEMAND 90000
#define MAX_STOCK 11000
#define MIN_STOCK 9000

int store_inventory[NUM_STORES];
pthread_mutex_t inventory_mutex = PTHREAD_MUTEX_INITIALIZER;

int generate_random(int min_value, int max_value) {
  return (rand() % (max_value - min_value + 1)) + min_value;
}

void *buyer(void *arg) {
  int *buyer_id = (int *)arg;
  int demand = generate_random(MIN_DEMAND, MAX_DEMAND);
  int stock = generate_random(MIN_STOCK, MAX_STOCK);
  printf("Buyer %d: Demanded %d with stock %d\n", *buyer_id, demand, stock);

  while (demand > 0) {
    pthread_mutex_lock(&inventory_mutex);
    int selected_store = rand() % NUM_STORES;
    if (store_inventory[selected_store] > 0) {
      int bought_stock = (demand > store_inventory[selected_store])
                             ? store_inventory[selected_store]
                             : demand;
      store_inventory[selected_store] -= bought_stock;
      demand -= bought_stock;
      printf("Buyer %d bought %d from store %d, remaining demand: %d\n",
             *buyer_id, bought_stock, selected_store, demand);
    }
    pthread_mutex_unlock(&inventory_mutex);
    sleep(2);
  }

  printf("Buyer %d finished shopping.\n", *buyer_id);
  pthread_exit(NULL);
}

void *restocker(void *arg) {
  while (1) {
    pthread_mutex_lock(&inventory_mutex);
    for (int i = 0; i < NUM_STORES; i++) {
      if (store_inventory[i] == 0) {
        store_inventory[i] = generate_random(MIN_STOCK, MAX_STOCK);
        printf("Restocker: Store %d restocked with %d goods.\n", i,
               store_inventory[i]);
      }
    }
    pthread_mutex_unlock(&inventory_mutex);
    sleep(5);
  }
}

void initialize_stores() {
  for (int i = 0; i < NUM_STORES; i++) {
    store_inventory[i] = generate_random(MIN_STOCK, MAX_STOCK);
  }
}

int main() {
  pthread_t buyers[NUM_BUYERS];
  pthread_t restocker_thread;

  srand(time(NULL));

  initialize_stores();

  int buyer_ids[NUM_BUYERS];
  for (int i = 0; i < NUM_BUYERS; i++) {
    buyer_ids[i] = i;
    pthread_create(&buyers[i], NULL, buyer, &buyer_ids[i]);
  }

  pthread_create(&restocker_thread, NULL, restocker, NULL);

  for (int i = 0; i < NUM_BUYERS; i++) {
    pthread_join(buyers[i], NULL);
  }

  pthread_cancel(restocker_thread);

  return 0;
}
