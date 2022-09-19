#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <omp.h>

#define GOLDEN_RATIO 618033
#define hash(v, M) ((GOLDEN_RATIO * (unsigned) v) % M)

#define TRUE 1
#define FALSE 0

#define key(A) (A.key)
#define NULL_ITEM_VALUE -1

#define order_key(A) (A.value)
#define greater(A, B) (order_key(A) > order_key(B))
#define exch(A, B) { Item tmp = A; A = B; B = tmp; }
#define cmp_exch(A, B) { if (greater(B, A)) exch(A, B); }

typedef int key_t;
typedef int value_t;
typedef int boolean;

typedef struct Item {
  key_t key;
  value_t value;
} Item;

typedef struct HashTable {
  int M;
  int N;
  Item * list;
} HashTable;

Item NULL_ITEM = { .key = -1, .value = NULL_ITEM_VALUE };

boolean is_null(Item * item) {
  return item->key == NULL_ITEM.key || item->value == NULL_ITEM.value;
}

Item create_item(key_t key, value_t value) {
  Item item = { .key = key, .value = value };

  return item;
}

HashTable hashtable_init(int max) {
  HashTable hash_table;

  hash_table.N = 0;
  hash_table.M = 2 * max;

  hash_table.list = (Item *) malloc (hash_table.M * sizeof(Item));

  for (int i = 0; i < hash_table.M; i++) {
    hash_table.list[i] = NULL_ITEM;
  }

  return hash_table;
}

void hashtable_insert(HashTable * ht, key_t key, value_t value) {
  Item new_item = create_item(key, value);

  int i = hash(key(new_item), ht->M);

  while(!is_null(&ht->list[i]))
    i = (i + 1) % ht->M;

  ht->list[i] = new_item;
  ht->N++;
}

Item * hashtable_search(HashTable * ht, key_t key) {
  int i = hash(key, ht->M);

  while(!is_null(&ht->list[i])) {
    if (key == key(ht->list[i]))
      return &ht->list[i];

    i = (i + 1) % ht->M;
  }

  return &NULL_ITEM;
}

boolean sorting_cmp(Item * a, Item * b) {
  if (a->value > b->value)
    return TRUE;
  if (a->value < b->value)
    return FALSE;
  if (a->key > b->key)
    return TRUE;
  if (a->key < b->key)
    return FALSE;

  return FALSE;
}

int partition(Item * v, int l, int r) {
  int j = l;
  Item c = v[r];

  for (int k = l; k < r; k++) {
    if (sorting_cmp(&v[k], &c)) {
      exch(v[k], v[j]);
      j++;
    }
  }

  exch(v[j], v[r]);

  return j;
}

void quicksortM3(Item * v, int l, int r) {
  int pivo_idx;
  int middle = (l + r) / 2;

  if (r <= l) return;

  cmp_exch(v[middle], v[r]);
  cmp_exch(v[l], v[middle]);
  cmp_exch(v[r], v[middle]);

  pivo_idx = partition(v, l, r);

  quicksortM3(v, l, pivo_idx - 1);

  quicksortM3(v, pivo_idx + 1, r);
}

void quickselect(Item * v, int l, int r, int k){
  int pivo_idx;
  int middle = (l + r) / 2;

  cmp_exch(v[middle], v[r]);
  cmp_exch(v[l], v[middle]);
  cmp_exch(v[r], v[middle]);

  pivo_idx = partition(v, l, r);

  if (pivo_idx == k) return;

  if (pivo_idx > k) quickselect(v, l, pivo_idx - 1, k);

  if (pivo_idx < k) quickselect(v, pivo_idx + 1, r, k);
}

void add_vote_to_candidate(HashTable * hash_table, Item * item, int vote) {
  if (is_null(item)) {
    hashtable_insert(hash_table, vote, 1);
  } else {
    item->value++;
  }
}

void add_vote_to_position(HashTable * hash_table, int vote, int * position_count) {
  Item * item = hashtable_search(hash_table, vote);

  (*position_count)++;

  add_vote_to_candidate(hash_table, item, vote);
}

void print_elected_president(HashTable * ht_president, int total_president_votes) {
  if (((ht_president->list[0].value * 100.0) / total_president_votes) >= 51.0) {
    printf("%d\n", ht_president->list[0].key);
  } else {
    printf("Segundo Turno\n");
  }
}

void print_elected_for_position(HashTable * ht_position, int to_elect) {
  for (int i = 0; i < to_elect; i++) {
    printf("%d%c", ht_position->list[i].key, i == to_elect - 1 ? '\n' : ' ');
  }
}

void order_to_elect_with_max_votes(HashTable * ht_position, int to_elect) {
  quickselect(ht_position->list, 0, ht_position->M - 1, to_elect - 1);
  quicksortM3(ht_position->list, 0, to_elect - 1);
}

void print_ht_list(HashTable * hash_table) {
  for (int i = 0; i < hash_table->M; i++) {
    Item item = hash_table->list[i];

    if (!is_null(&item)) {
      printf("%d => %d ", item.key, item.value);
    } else {
      printf("null ");
    }
  }
  printf("\n");
}

int main(void) {
  char str_vote[6];
  int total_votes = 0;
  int invalid_votes = 0;
  int to_elect_presidents = 2;
  int to_elect_senator, to_elect_federal_deputie, to_elect_state_deputie;
  int total_president_votes = 0, total_senator_votes = 0, total_federal_deputie_votes = 0, total_state_deputie_votes = 0;

  HashTable ht_president = hashtable_init(99);
  HashTable ht_senator = hashtable_init(1000);
  HashTable ht_federal_deputie = hashtable_init(10000);
  HashTable ht_state_deputie = hashtable_init(100000);

  scanf("%d %d %d", &to_elect_senator, &to_elect_federal_deputie, &to_elect_state_deputie);

  while (scanf("%s", str_vote) != EOF) {
    int vote = atoi(str_vote);
    int str_vote_len = strlen(str_vote);

    if (vote < 0) {
      invalid_votes++;
      continue;
    }

    if (str_vote_len == 2) add_vote_to_position(&ht_president, vote, &total_president_votes);
    else if (str_vote_len == 3) add_vote_to_position(&ht_senator, vote, &total_senator_votes);
    else if (str_vote_len == 4) add_vote_to_position(&ht_federal_deputie, vote, &total_federal_deputie_votes);
    else if (str_vote_len == 5) add_vote_to_position(&ht_state_deputie, vote, &total_state_deputie_votes);
    // na especificação diz que só inválido se for negativo
    // else invalid_votes++;
  }

  total_votes = (
    total_president_votes + total_senator_votes + total_federal_deputie_votes + total_state_deputie_votes
  );

#pragma omp parallel num_threads(4)
{
  #pragma omp taskgroup
  {
    #pragma omp task
      order_to_elect_with_max_votes(&ht_president, to_elect_presidents);

    #pragma omp task
      order_to_elect_with_max_votes(&ht_senator, to_elect_senator);

    #pragma omp task
      order_to_elect_with_max_votes(&ht_federal_deputie, to_elect_federal_deputie);

    #pragma omp task
      order_to_elect_with_max_votes(&ht_state_deputie, to_elect_state_deputie);
  }
}

  // output

  printf("%d %d\n", total_votes, invalid_votes);

  print_elected_president(&ht_president, total_president_votes);

  print_elected_for_position(&ht_senator, to_elect_senator);

  print_elected_for_position(&ht_federal_deputie, to_elect_federal_deputie);

  print_elected_for_position(&ht_state_deputie, to_elect_state_deputie);

  return 0;
}
