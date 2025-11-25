#ifndef BILLLIB_H
#define BILLLIB_H

#include <stdio.h>

#define ACCEPTEDMETHODS 6
#define MAX_NAME_LEN 20
#define MAX_NOTE_LEN 50
#define DATE_LEN 11 // 10 chars + null terminator

enum paymentType { CREDIT, DEBIT, MONEYORDER, CASH, CHECK, PAYMENTPLANS };

enum BillCriteria { BY_AMOUNT, BY_DATE }; // TODO: ADD MORE CRITERIA

enum sortField { AMOUNT, DATE }; // TODO: ADD MORE CRITERIA

extern char paymentMethod[ACCEPTEDMETHODS][15];

typedef struct sBill {
  char category[MAX_NAME_LEN];
  char provider[MAX_NAME_LEN];
  float total;
  enum paymentType payment;
  char paymentMethod[15];
  char date[DATE_LEN];
  char billingPeriod[MAX_NAME_LEN];
  int taxRelevance;
  char notes[MAX_NOTE_LEN];
  sBill *next;
  sBill *prev;

} sBill;

typedef struct {
  sBill *head;
  sBill *tail;
  int size;

} sBill_LL;

void init_list(sBill_LL *list);
void add_bill(sBill_LL *list, sBill *newBill);
void edit_bill(sBill_LL *list, int index);
void delete_bill(sBill_LL *list, int index);
void clear_list(sBill_LL *list);
void view_bills(const sBill_LL *list);
void filter_bills(sBill_LL *list, int criteria);
void sort_bills(sBill_LL *list, int field);
void generate_report(const sBill_LL *list);
void save_bill(const sBill_LL *list);
void load_bill(const sBill_LL *list);
void export_csv(const sBill_LL *list);
void save_bin(const sBill_LL *list);
void load_bin(const sBill_LL *list);
void print_menu(const sBill_LL *list);
void read_line(char *buffer, int size);
int read_int_in_range(int min, int max);
float read_float_positive(void);

#endif // BILLLIB_H