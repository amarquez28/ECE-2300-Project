#include <stdio.h>
#include <stdlib.h>
#include "BillLib.h"

int main(void){
    sBill_LL list;
    init_list(&list);

    int running = 1;

    while (running){
        print_menu(&list);

        int choice;
        if(scanf("%d", &choice) != 1){
            // clear bad inputs
            int c;
            while ((c = getchar()) != '\n' && c != EOF){}
            printf("Invalid input.\n");
            continue;
        }
        // consume leftover newline
        int c;
        while ((c = getchar()) != '\n' && c != EOF){}

        switch (choice) {
            case 1: {
                sBill *bill = create_bill_from_input();
                if(bill){
                    add_bill(&list, bill);
                    printf("Bill added.\n");
                }
                break;
            }
            case 2: {
                // Edit bill
                view_bills(&list);
                if(list.size == 0) break;
                printf("Enter bill number to edit:\n");
                int idx = read_int_in_range(1, list.size);
                edit_bill(&list, idx);
                break;
            }
            case 3: {
                // Delete bill
                view_bills(&list);
                if (list.size == 0) break;
                printf("Enter bill number to delete:\n");
                int idx = read_int_in_range(1, list.size);
                delete_bill(&list, idx);
                break;
            }
            case 4: {
                // View & filter
                printf("1) View all\n2) Filter by amount\n3) Filter by date\n");
                int sub = read_int_in_range(1, 3);
                if (sub == 1){
                    view_bills(&list);
                }
                else if (sub == 2){
                    filter_bills(&list, BY_AMOUNT);
                }
                else{
                    filter_bills(&list, BY_DATE);
                }
                break;
            }
            case 5: {
                // Sort
                printf("Sort by: \n1) Amount\n2) Date\n");
                int sub = read_int_in_range(1, 2);
                if (sub == 1)
                    sort_bills(&list, AMOUNT);
                else
                    sort_bills(&list, DATE);
                break;
            }
            case 6:
            // Reports & summaries
            generate_report(&list);
            break;

            case 7:
            // Save (binary)
            save_bin(&list);
            break;

            case 8:
            // Load (binary)
            load_bin(&list);
            break;

            case 9:
            // Export CSV
            export_csv(&list);
            break;

            case 0:
            running = 0;
            break;

            default:
                printf("Unknown option.\n");
                break;
        }
    }
    //Ask to save before quitting
    printf("Save bills to binary file before quitting? (1 = Yes, 0 = No)\n");
    int saveBeforeExit = read_int_in_range(0,1);
    if (saveBeforeExit){
        save_bin(&list);
    }

    clear_list(&list);
    printf("Goodbye!\n");
    return 0;
}