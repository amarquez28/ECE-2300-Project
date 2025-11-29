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
    if (sscanf(line, "%f", &value) == 1 && value >= 0.0f){
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
sBill *create_bill_from_input(void){
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

/* Core Functions*/
void init_list(sBill_LL *list){
  if (!list) return;
  list->head = list->tail = NULL;
  list->size = 0;
}

void add_bill(sBill_LL *list, sBill *newBill){
  if(!list || !newBill) return;
  newBill->next = NULL;
  newBill->prev = list->tail;

  if(list->tail){
    list->tail->next = newBill;
  }
  else{
    list->head = newBill;
  }
  list->tail = newBill;
  list->size++;
}

void delete_bill(sBill_LL *list, int index){
  if(!list || list->size == 0){
    printf("No bills to delete.\n");
    return;
  }
  sBill *target = get_bill_at(list, index);
  if (!target){
    printf("Invalid index.\n");
    return;
  }
  printf("Are you sure you wan to delete bill #%d? (1 = Yes, 0 = No)\n", index);
  int confirm = read_int_in_range(0, 1);
  if (!confirm){
    printf("Delete canceled.\n");
    return;
  }
  if(target->prev)
    target->prev->next = target->next;
  else
    list->head = target->next;

  if(target->next)
    target->next->prev = target->prev;
  else
    list->tail = target->prev;

  free(target);
  list->size--;

  printf("Bill deleted.\n");
}

void edit_bill(sBill_LL *list, int index){
  if (!list || list->size == 0){
    printf("No bills to edit.\n");
    return;
  }
  sBill *bill = get_bill_at(list, index);
  if (!bill){
    printf("Invalid index.\n");
    return;
  }
  printf("Editing bill #%d:\n", index);
  printf("Leave field empty to keep current value.\n\n");

  char buffer[128];

  printf("Category (current: %s):\n", bill->category);
  read_line(buffer, sizeof(buffer));
  if(strlen(buffer) > 0){
    strncpy(bill->category, buffer, MAX_NAME_LEN - 1);
    bill->category[MAX_NAME_LEN - 1] = '\0';
  }

  printf("Privider (current: %s):\n", bill->provider);
  read_line(buffer, sizeof(buffer));
  if (strlen(buffer) > 0) {
    strncpy(bill->provider, buffer, MAX_NAME_LEN - 1);
    bill->provider[MAX_NAME_LEN - 1] = '\0';
  }

  printf("Total (current: %.2f) (leave emtpy to keep):\n", bill->total);
  read_line(buffer, sizeof(buffer));
  if(strlen(buffer) > 0){
    float t;
    if (sscanf(buffer, "%f", &t) == 1 && t >= 0.0f){
      bill->total = t;
    }
  }

  printf("Payment type (current: %d) or empty to keep:\n", bill->payment);
  printf("0) CREDIT\n1) DEBIT\n2) MONEY ORDER\n3) CASH\n4) CHECK\n5) PAYMENT PLANS\n");
  read_line(buffer, sizeof(buffer));
  if(strlen(buffer) > 0){
    int pt;
    if(sscanf(buffer, "%d", &pt) == 1 && pt >= 0 && pt <= 5){
      bill->payment = (enum paymentType)pt;
    }
  }

  printf("Payment method (current: %s) or empty to keep:\n", bill->paymentMethod);
  read_line(buffer, sizeof(buffer));
  if (strlen(buffer) > 0){
    strncpy(bill->paymentMethod, buffer, sizeof(bill->paymentMethod) - 1);
    bill->paymentMethod[sizeof(bill->paymentMethod) - 1] = '\0';
  }

  printf("Date (current: %s) or empty to keep:\n", bill->date);
  read_line(buffer, sizeof(buffer));
  if (strlen(buffer) > 0) {
    strncpy(bill->date, buffer, DATE_LEN - 1);
    bill->date[DATE_LEN - 1] = '\0';
  }

  printf("Billing period (current: %s) or empty to keep:\n", bill->billingPeriod);
  read_line(buffer, sizeof(buffer));
  if(strlen(buffer) > 0){
    strncpy(bill->billingPeriod, buffer, MAX_NAME_LEN - 1);
    bill->billingPeriod[MAX_NAME_LEN - 1] = '\0';
  }

  printf("Tax relevant (current: %d) or empty to keep:\n", bill->taxRelevance);
  read_line(buffer, sizeof(buffer));
  if (strlen(buffer) > 0){
    int tx;
    if (sscanf(buffer, "%d", &tx) == 1 && (tx == 0 || tx == 1)){
      bill->taxRelevance = tx;
    }
  }

  printf("Notes (current: %s) or empty to keep: \n", bill->notes);
  read_line(buffer, sizeof(buffer));
  if (strlen(buffer) > 0){
    strncpy(bill->notes, buffer, MAX_NOTE_LEN - 1);
    bill->notes[MAX_NAME_LEN - 1] = '\0';
  }

  printf("Bill updated.\n");
}

void clear_list(sBill_LL *list){
  if(!list) return;
  sBill *curr = list->head;
  while (curr){
    sBill *next = curr->next;
    free(curr);
    curr = next;
  }
  list->head = list->tail = NULL;
  list->size = 0;
}

void view_bills(const sBill_LL *list) {
    if (!list || list->size == 0) {
        printf("No bills recorded.\n");
        return;
    }

    printf("\n---- Bills (%d total) ----\n", list->size);
    int index = 1;
    sBill *curr = list->head;
    while (curr) {
        printf("#%d) [%s] %s | $%.2f | %s | %s | Tax:%s\n",
               index,
               curr->category,
               curr->provider,
               curr->total,
               curr->date,
               curr->billingPeriod,
               curr->taxRelevance ? "Yes" : "No");
        printf("    Payment: type=%d, method=%s\n", curr->payment, curr->paymentMethod);
        printf("    Notes: %s\n", curr->notes);
        curr = curr->next;
        index++;
    }
    printf("--------------------------\n");
}

void filter_bills(sBill_LL *list, int criteria) {
  switch (criteria) {
  case BY_AMOUNT: {
    float min, max;
    char line[64];

    printf("Min total amount:\n");
    min = read_float_positive();

    printf("Max total amount (0 for no max):\n");
    fgets(line, sizeof(line), stdin);
    if (sscanf(line, "%f", &max) != 1 || max <= 0){
      max = 1e9f;
    }

    printf("\nFiltered by amount [%.2f - %.2f]:\n", min, max);
    int count = 0;
    sBill *curr = list->head;
    while (curr){
      if(curr->total >= min && curr->total <= max){
        count++;
        printf("#%d) [%s] %s | $%.2f | %s\n", count, curr->category, curr->provider, curr->total, curr->date);
      }
      curr = curr->next;
    }
    printf("Matched %d record(s).\n", count);
  }
    break;
  case BY_DATE: {
    char start[DATE_LEN], end[DATE_LEN];
    printf("Start date (YYYY-MM-DD):\n");
    read_line(start, DATE_LEN);
    printf("End date (YYYY-MM-DD):\n");
    read_line(end, DATE_LEN);

    printf("\nFiltered by date [%s - %s]:\n", start, end);
    int count = 0;
    sBill *curr = list->head;
    while (curr){
      if (compare_dates(curr->date, start) >= 0 &&
          compare_dates(curr->date, end) <= 0){
            count++;
            printf("#%d) [%s] %s | $%.2f | %s\n", count, curr->category, curr->provider, curr->total, curr->date);
          }
          curr = curr->next;
    }
    printf("Matched %d record(s).\n", count);
    break;
  }
  default:
    printf("Unknown filter criteria.\n");
    break;
  }
}

void sort_bills(sBill_LL *list, int field) {
  if (!list || list->size < 2){
    printf("Not enough bills to sort.\n");
    return;
  }

  int swapped;
  do{
    swapped = 0;
    sBill *curr = list->head;
    while (curr && curr->next){
      int cmp = 0;
      switch (field){
        case AMOUNT:
          if(curr->total > curr->next->total)
            cmp = 1;
          break;
        case DATE:
          if(compare_dates(curr->date, curr->next->date) > 0)
            cmp = 1;
          break;
        default:
          break;
      }
      if (cmp){
        swap_bill_data(curr, curr->next);
        swapped = 1;
      }
      curr = curr->next;
    }
  } while(swapped);
  printf("Bills sorted.\n");
}

void generate_report(const sBill_LL *list){
  if (!list || list->size == 0) {
        printf("No bills to report.\n");
        return;
    }

    printf("\n--- Report: By Month (YYYY-MM) ---\n");

    // naive fixed-size arrays for demo
    #define MAX_GROUPS 100
    char months[MAX_GROUPS][8]; // "YYYY-MM" + null
    int monthCount = 0;
    int monthRecords[MAX_GROUPS] = {0};
    float monthTotals[MAX_GROUPS] = {0.0f};

    sBill *curr = list->head;
    while (curr) {
        char key[8] = {0};
        strncpy(key, curr->date, 7); // YYYY-MM

        int idx = -1;
        for (int i = 0; i < monthCount; i++) {
            if (strcmp(months[i], key) == 0) {
                idx = i;
                break;
            }
        }
        if (idx == -1 && monthCount < MAX_GROUPS) {
            idx = monthCount++;
            strncpy(months[idx], key, 7);
            months[idx][7] = '\0';
        }
        if (idx != -1) {
            monthRecords[idx]++;
            monthTotals[idx] += curr->total;
        }

        curr = curr->next;
    }

    printf("Month      Count   Total      Average\n");
    printf("--------------------------------------\n");
    for (int i = 0; i < monthCount; i++) {
        float avg = monthRecords[i] ? (monthTotals[i] / monthRecords[i]) : 0.0f;
        printf("%-10s %-7d $%-9.2f $%.2f\n",
               months[i], monthRecords[i], monthTotals[i], avg);
    }

    // By Category
    printf("\n--- Report: By Category ---\n");
    char cats[MAX_GROUPS][MAX_NAME_LEN];
    int catCount = 0;
    int catRecords[MAX_GROUPS] = {0};
    float catTotals[MAX_GROUPS] = {0.0f};
    float catMin[MAX_GROUPS];
    float catMax[MAX_GROUPS];

    for (int i = 0; i < MAX_GROUPS; i++) {
        catMin[i] = 1e9f;
        catMax[i] = 0.0f;
    }

    curr = list->head;
    while (curr) {
        int idx = -1;
        for (int i = 0; i < catCount; i++) {
            if (strcmp(cats[i], curr->category) == 0) {
                idx = i;
                break;
            }
        }
        if (idx == -1 && catCount < MAX_GROUPS) {
            idx = catCount++;
            strncpy(cats[idx], curr->category, MAX_NAME_LEN - 1);
            cats[idx][MAX_NAME_LEN - 1] = '\0';
        }

        catRecords[idx]++;
        catTotals[idx] += curr->total;
        if (curr->total < catMin[idx]) catMin[idx] = curr->total;
        if (curr->total > catMax[idx]) catMax[idx] = curr->total;

        curr = curr->next;
    }

    printf("Category            Count   Total      Min       Max       Avg\n");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < catCount; i++) {
        float avg = catRecords[i] ? (catTotals[i] / catRecords[i]) : 0.0f;
        printf("%-20s %-7d $%-9.2f $%-8.2f $%-8.2f $%.2f\n",
               cats[i], catRecords[i], catTotals[i],
               (catMin[i] == 1e9f ? 0.0f : catMin[i]),
               catMax[i],
               avg);
    }
}

void save_bill(const sBill_LL *list){
  if (!list) return;

    FILE *fp = fopen("bills.txt", "w");
    if (!fp) {
        printf("Could not open bills.txt for writing.\n");
        return;
    }

    sBill *curr = list->head;
    while (curr) {
        fprintf(fp, "%s|%s|%.2f|%d|%s|%s|%s|%d|%s\n",
                curr->category,
                curr->provider,
                curr->total,
                curr->payment,
                curr->paymentMethod,
                curr->date,
                curr->billingPeriod,
                curr->taxRelevance,
                curr->notes);
        curr = curr->next;
    }

    fclose(fp);
    printf("Bills saved to bills.txt\n");
}

void load_bill(sBill_LL *list){
  if (!list) return;

    FILE *fp = fopen("bills.txt", "r");
    if (!fp) {
        printf("No bills.txt file found.\n");
        return;
    }

    clear_list(list);

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        sBill *bill = (sBill *)malloc(sizeof(sBill));
        if (!bill) break;
        memset(bill, 0, sizeof(sBill));

        char *token = strtok(line, "|");
        if (!token) { free(bill); continue; }
        strncpy(bill->category, token, MAX_NAME_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) { free(bill); continue; }
        strncpy(bill->provider, token, MAX_NAME_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) { free(bill); continue; }
        bill->total = (float)atof(token);

        token = strtok(NULL, "|");
        if (!token) { free(bill); continue; }
        bill->payment = (enum paymentType)atoi(token);

        token = strtok(NULL, "|");
        if (!token) { free(bill); continue; }
        strncpy(bill->paymentMethod, token, sizeof(bill->paymentMethod) - 1);

        token = strtok(NULL, "|");
        if (!token) { free(bill); continue; }
        strncpy(bill->date, token, DATE_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) { free(bill); continue; }
        strncpy(bill->billingPeriod, token, MAX_NAME_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) { free(bill); continue; }
        bill->taxRelevance = atoi(token);

        token = strtok(NULL, "\n");
        if (!token) token = "";
        strncpy(bill->notes, token, MAX_NOTE_LEN - 1);

        bill->next = bill->prev = NULL;
        add_bill(list, bill);
    }

    fclose(fp);
    printf("Bills loaded from bills.txt\n");
}

void export_csv(const sBill_LL *list){
  if (!list) return;

    FILE *fp = fopen("bills.csv", "w");
    if (!fp) {
        printf("Could not open bills.csv for writing.\n");
        return;
    }

    fprintf(fp,
            "Category,Provider,Total,PaymentType,PaymentMethod,Date,BillingPeriod,TaxRelevance,Notes\n");

    sBill *curr = list->head;
    while (curr) {
        fprintf(fp,
                "\"%s\",\"%s\",%.2f,%d,\"%s\",\"%s\",\"%s\",%d,\"%s\"\n",
                curr->category,
                curr->provider,
                curr->total,
                curr->payment,
                curr->paymentMethod,
                curr->date,
                curr->billingPeriod,
                curr->taxRelevance,
                curr->notes);
        curr = curr->next;
    }

    fclose(fp);
    printf("Bills exported to bills.csv\n");
}


void save_bin(const sBill_LL *list){
  if (!list) return;

    FILE *fp = fopen("bills.bin", "wb");
    if (!fp) {
        printf("Could not open bills.bin for writing.\n");
        return;
    }

    fwrite(&list->size, sizeof(int), 1, fp);
    sBill *curr = list->head;
    while (curr) {
        fwrite(curr, sizeof(sBill), 1, fp);
        curr = curr->next;
    }

    fclose(fp);
    printf("Bills saved to bills.bin\n");
}

void load_bin(sBill_LL *list){
  if (!list) return;

    FILE *fp = fopen("bills.bin", "rb");
    if (!fp) {
        printf("No bills.bin file found.\n");
        return;
    }

    clear_list(list);

    int count = 0;
    fread(&count, sizeof(int), 1, fp);

    for (int i = 0; i < count; i++) {
        sBill *bill = (sBill *)malloc(sizeof(sBill));
        if (!bill) break;
        if (fread(bill, sizeof(sBill), 1, fp) != 1) {
            free(bill);
            break;
        }
        bill->next = bill->prev = NULL;
        add_bill(list, bill);
    }

    fclose(fp);
    printf("Bills loaded from bills.bin\n");
}

/*Menu UI*/
void print_menu(const sBill_LL *list){
  printf("\n-$$----Bill Organizer----$$-\n");
    printf("|                          |\n");
    printf("| 1) Add bill              |\n");
    printf("| 2) Edit bill             |\n");
    printf("| 3) Delete bill           |\n");
    printf("| 4) View & filter bill    |\n");
    printf("| 5) Sort bill             |\n");
    printf("| 6) Reports & summaries   |\n");
    printf("| 7) Save (binary)         |\n");
    printf("| 8) Load (binary)         |\n");
    printf("| 9) Export CSV            |\n");
    printf("| 0) Quit                  |\n");
    printf("|                          |\n");
    printf("---------------------------\n");
    printf("Choice: ");
}