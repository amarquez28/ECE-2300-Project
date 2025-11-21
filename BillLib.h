#ifndef BILLLIB_H
#define BILLLIB_H
#define ACCEPTEDMETHODS 6

enum paymentType{CREDIT, DEDIT, MONEYORDER, CASH, CHECK, PAYMENTPLANS};

enum BillCriteria{BY_AMOUNT,BY_DATE};//TODO: ADD MORE CRITERIA

enum sortField{AMOUNT,DATE};//TODO: ADD MORE CRITERIA


char paymentMethod[ACCEPTEDMETHODS][15] = {
    "Apple Pay ",
    "Google Pay",
    "Money Order ",
    "Cash ",
    "Card ",
    "Klarna "
};

typedef struct{
    char category [20];
    char provider [20];
    float total;
    enum paymentType payment;
    char paymentMethod[15];
    char date[10];
    char billingPeriod[20];
    int taxRelevance;
    char notes[50];
    sBill *next;
    sBill *prev;

}sBill;

typedef struct{
    sBill *head;
    sBill *tail;
    int size;

}sBill_LL;


void init_list(sBill_LL *list);
void add_bill(sBill_LL *list, sBill newBill);
void edit_bill(sBill_LL *list, int index);
void delete_bill(sBill_LL *list, int index);
void filter_bills(sBill_LL *list, int criteria);
void sort_bills(sBill_LL *list, int field);
void generate_report(sBill_LL *list);
void save_bill(sBill_LL *list);
void load_bill(sBill_LL *list);
void export_csv(sBill_LL *list);
void save_bin(sBill_LL *list);
void load_bin(sBill_LL *list);
void print_menu(sBill_LL *list);



#endif //BILLLIB_H