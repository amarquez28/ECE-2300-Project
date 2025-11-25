#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BillLib.h"

char paymentMethod[ACCEPTEDMETHODS][15] = {
    "Apple Pay ", "Google Pay", "Money Order ", "Cash ", "Card ", "Klarna "};

/*Helper functions*/

void read_line(char *buffer, int size){
  if(fgets(buffer, size, stdin) != NULL){
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n'){
      buffer[len - 1] = '\0';
    }
  }
}

int read_int_in_range(int min, int max){
  int value;
  char line[64];

  while(1){
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)){
      continue;
    }
    if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max){
      return value;
    }
    printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
  }
}

float read_float_positive(void){
  float value;
  char line[64];

  while(1){
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)){
      continue;
    }
    if (sscan(line, "%f", &value) == 1 && value >= 0.0f){
      return value;
    }
    printf("Invalid amount. Please enter a positive number.\n");
  }
}

// Get pointer to bill at 1-based index or NULL
static sBill *get_bill_at(const sBill_LL *list, int index){
  if (!list || index < 1 || index > list->size) return NULL;
  
  sBill *curr = list->head;
  int i = 1;
  while (curr && i < index){
    curr = curr->next;
    i++;
  }
  return curr;
}

/*Compare "YYYY-MM-DD" strings, safe as plain strcmp*/
static int compare_dates(const char *d1, const char *d2){
  return strcmp(d1, d2);
}

/*Swap all fields between two nodes for sorting*/
static void swap_bill_data(sBill *a, sBill *b){
  sBill temp = *a;
  // Keep the pointers of the nodes themselves
  temp.next = temp.prev = NULL;

  // Copy b into a
  char *tmp_next = (char *)a->next;
  char *tmp_prev = (char *)a->prev;

  *a = *b;
  a->next = (sBill *)tmp_next;
  a->prev = (sBill *)tmp_prev;

  // Copy original a into b
  tmp_next = (char *)b->next;
  tmp_prev = (char *)b->prev;
  *b = temp;
  b->next = (sBill *)tmp_next;
  b->prev = (sBill *)tmp_prev;
}

/* Creating a new bill for the user*/
static sBill *create_bill_from_input(void){
  sBill *bill = (sBill *)malloc(sizeof(sBill));
  if(!bill){
    printf("Memory allocation failed.\n");
    return NULL;
  }
  memset(bill, 0, sizeof(sBill));

  printf("Category (e.g., Water, Electricity):\n");
  read_line(bill->category, MAX_NAME_LEN);

  printf("Privider (e.g., City Water, Electric Co):\n");
  read_line(bill->provider, MAX_NAME_LEN);

  printf("Total amount:\n");
  bill->total = read_float_positive();

  printf("Payment type:\n");
  printf("0) CREDIT\n1) DEBIT\n2) MONEY ORDER\n3) CASH\n4) CHECK\n5) PAYMENT PLANS\n");
  bill->payment = (enum paymentType)read_int_in_range(0, 5);

  printf("Payment method\n");
    for (int i = 0; i < ACCEPTEDMETHODS; i++){
      printf("%d) %s\n", i, paymentMethod[i]);
    }
  
  int methodIndex = read_int_in_range(0, ACCEPTEDMETHODS - 1);
  strncpy(bill->paymentMethod, paymentMethod[methodIndex], sizeof(bill->paymentMethod) - 1);
  bill->paymentMethod[sizeof(bill->paymentMethod) - 1] = '\0';

  printf("Date (YYYY-MM-DD):\n");
  read_line(bill->date, DATE_LEN);

  printf("Billing period (e.g., Jan 2025):\n");
  read_line(bill->billingPeriod, MAX_NAME_LEN);

  printf("Tax relevant? (1 = Yes, 0 = No):\n");
  bill->taxRelevance = read_int_in_range(0,1);

  printf("Notes (max %d charaacters):\n", MAX_NOTE_LEN - 1);
  read_line(bill->notes, MAX_NOTE_LEN);

  bill->next = bill->prev = NULL;
  return bill;
}

void filter_bills(sBill_LL *list, int criteria) {
  switch (criteria) {
  case BY_AMOUNT:
    /* code */
    break;
  case BY_DATE:
    /* code */
    break;
  default:
    break;
  }
}

void sort_bills(sBill_LL *list, int field) {
  switch (field) {
  case AMOUNT:
    /* code */
    break;
  case DATE:
    /* code */
    break;
  default:
    break;
  }
}
