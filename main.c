#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#define ACC_FILE "accounts.txt"
#define USER_FILE "users.txt"

// --- SHARED DATA STRUCTURES ---
const char *months[] = {
    "January", "February", "March", "April",
    "May", "June", "July", "August",
    "September", "October", "November", "December"
};
typedef struct {
    int month;
    int year;
} Date;

typedef struct {
    char account_no[20];
    char name[50];
    char mobile[20];
    char emailaddress[60];
    float balance;
    Date dateopened;
    char status[14];
} account;

// --- GLOBAL VARIABLES ---
account *accounts = NULL;
int account_count = 0;

// --- FUNCTION PROTOTYPES ---
void login();
int validate_username_pass(char *username, char *password);
void load_data();
void save_data();
void menu();
void quit();
void ADD();
void delete_account();
void modify();
void query();
void ADVANCED_SEARCH();
void change_status();
void WITHDRAW();
void DEPOSIT();
void TRANSFER();
void REPORT();
void PRINT();
void format_name(char *name);
int check_ac_number(char *num);
int check_unique(char* num);
void print_account(account ac);
void log_transaction(char *ac_num, char *type, float amount);
int check_daily_limit(char *ac_num, float new_amount);
int is_valid_email(char *email);
int check_balance(char*balance);
int check_mobile(char* mobile);
void DELETE_INACTIVE_AC();
void DELETE_MULTIBLE();
void DELETE_BY_DATE();
void SORT_BY_NAME();
void SORT_BY_BALANCE();
void SORT_BY_DATE();
void SORT_BY_STATUS();
int check_to_save();
int check_name(char *name);


int main() {
    system("color 7D");

    login();

    load_data();

    menu();
    return 0;
}

// =============================================================
//      AUTHENTICATION & FILES
// =============================================================
void login() {
    int choice;
    while(1) {
        printf("\n=== BANK SYSTEM ===\n");
        printf("1. LOGIN\n");
        printf("2. QUIT\n");
        printf("Select: ");
        scanf("%d", &choice);
        getchar();

        if(choice == 2) exit(0);
        if(choice == 1) break;
        printf("Invalid choice.\n");
    }

    // 3 Attempts Logic
    int attempts = 0;
    char username[100], password[100];

    while (attempts < 3) {
        printf("\n--- LOGIN (Attempt %d/3) ---\n", attempts + 1);

        printf("Enter username: ");
        fgets(username, 99, stdin);
        username[strcspn(username, "\n")] = '\0';

        printf("Enter password: ");
        fgets(password, 99, stdin);
        password[strcspn(password, "\n")] = '\0';

        if (validate_username_pass(username, password)) {
            printf("Login successful.\n");
            return;
        } else {
            printf("Invalid credentials. Please try again.\n");
            attempts++;
        }
    }

    printf("Too many failed attempts. Exiting system security.\n");
    exit(1);
}
int validate_username_pass(char *username, char *password) {
    char line[200];
    int found = 0;
    FILE *ptr = fopen(USER_FILE, "r");

        // Auto-create for testing if missing
    if (!ptr) {
        ptr = fopen(USER_FILE, "w");
        if (!ptr) {
            printf("Error: Could not create users file.\n");
            return 0;
        }
        fprintf(ptr, "admin 123\n");
        fclose(ptr);
        ptr = fopen(USER_FILE, "r");
        printf("[DEBUG] Created 'users.txt' (User: admin, Pass: 123)\n");
    }

    while (fgets(line, sizeof(line), ptr)) {
        line[strcspn(line, "\r\n")] = 0;
        char *u = strtok(line, " ");
        char *p = strtok(NULL, " ");

        if (u && p && strcmp(u, username) == 0 && strcmp(p, password) == 0) {
            found = 1;
            break;
        }
    }
    fclose(ptr);

    return found;
}


void load_data() {
    FILE *fp = fopen(ACC_FILE, "r");
    if (!fp) {
        printf("No accounts file found. Starting fresh.\n");
        accounts = malloc(10 * sizeof(account));
        return;
    }

    int capacity = 10;
    account_count = 0;
    accounts = malloc(capacity * sizeof(account));
    char line[300];

    while (fgets(line, sizeof(line), fp)) {
        if (account_count >= capacity) {
            capacity += 10;
            accounts = realloc(accounts, capacity * sizeof(account));
        }

        account a;
        char *token = strtok(line, ",");
        if(token) strcpy(a.account_no, token);

        token = strtok(NULL, ",");
        if(token) strcpy(a.name, token);

        token = strtok(NULL, ",");
        if(token) strcpy(a.emailaddress, token);

        token = strtok(NULL, ",");
        if(token) a.balance = atof(token);

        token = strtok(NULL, ",");
        if(token) strcpy(a.mobile, token);

        token = strtok(NULL, ",");
        if(token) sscanf(token, "%d-%d", &a.dateopened.month, &a.dateopened.year);

        token = strtok(NULL, "\n");
        if(token) {
             if(token[0] == ' ') strcpy(a.status, token+1);
             else strcpy(a.status, token);
        }
        accounts[account_count++] = a;
    }
    fclose(fp);
    printf("Loaded %d accounts.\n", account_count);
}

void save_data() {
    FILE *f = fopen(ACC_FILE, "w");
    if (!f) return;

    for (int i = 0; i < account_count; i++) {
        fprintf(f, "%s,%s,%s,%.2f,%s,%d-%d,%s\n",
            accounts[i].account_no, accounts[i].name,
            accounts[i].emailaddress, accounts[i].balance,
            accounts[i].mobile, accounts[i].dateopened.month,
            accounts[i].dateopened.year, accounts[i].status);
    }
    fclose(f);
}

void quit() {
    save_data();
    printf("Data saved. Goodbye.\n");
    exit(0);
}


void ADD() {
    if (account_count % 10 == 0) {
        accounts = realloc(accounts, (account_count + 10) * sizeof(account));
    }
    account new_ac;

    do {
        printf("Enter account number: ");
        fgets(new_ac.account_no, sizeof(new_ac.account_no), stdin);
        new_ac.account_no[strcspn(new_ac.account_no, "\n")] = 0;
    } while (!check_ac_number(new_ac.account_no) || !check_unique(new_ac.account_no));

do {
        printf("Enter name: ");
        fgets(new_ac.name, sizeof(new_ac.name), stdin);
    } while (!check_name(new_ac.name));

    format_name(new_ac.name);

    do {
        printf("Enter email: ");
        fgets(new_ac.emailaddress, sizeof(new_ac.emailaddress), stdin);
        new_ac.emailaddress[strcspn(new_ac.emailaddress, "\n")] = 0; // Remove newline

        if (!is_valid_email(new_ac.emailaddress)) {
            printf("Invalid email format! (Example: user@mail.com)\n");
        }
    } while (!is_valid_email(new_ac.emailaddress));

    char balanceStr[50];
    do {
        printf("Enter balance: ");
        fgets(balanceStr, sizeof(balanceStr), stdin);

    } while (!check_balance(balanceStr));

    new_ac.balance = atof(balanceStr);
    do {
        printf("Enter mobile (11 digits): ");
        fgets(new_ac.mobile, sizeof(new_ac.mobile), stdin);
    } while (!check_mobile(new_ac.mobile));


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    new_ac.dateopened.month = tm.tm_mon + 1;
    new_ac.dateopened.year = tm.tm_year + 1900;
    strcpy(new_ac.status, "active");


    if (!check_to_save()){
        printf("Account creation canceled.\n");
        return;
    }
    accounts[account_count] = new_ac;
    account_count++;
    save_data();
    printf("Account added successfully!\n");
}

void delete_account() {
    char accNum[20];
    printf("Enter Account Number to delete: ");
    scanf("%s", accNum);
    getchar();

    int index = -1;
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].account_no, accNum) == 0) { index = i; break; }
    }

    if (index == -1) { printf("Not found.\n"); return; }
    if (accounts[index].balance > 0) { printf("Error: Balance must be 0.\n"); return; }


    if (check_to_save()) {
        for (int i = index; i < account_count - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        account_count--;

        save_data();
        printf("Account deleted successfully.\n");
    } else {
        printf("Deletion discarded.\n");
    }
}

void modify() {
    char accNum[20];
    account temp_ac;
    printf("Enter Account Number to modify: ");
    scanf("%s", accNum);
    getchar();

    int index = -1;
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].account_no, accNum) == 0) { index = i; break; }
    }

    if (index == -1) { printf("Not found.\n"); return; }
strcpy(temp_ac.account_no, accounts[index].account_no);


     printf("Old Name: %s\n", accounts[index].name); // Show old name

    do {
        printf("New Name: ");
        fgets(temp_ac.name, 50, stdin);
    } while (!check_name(temp_ac.name));

    format_name(temp_ac.name);
    int validnum = 0;

    do {
        printf("Old Phone Number: %s. New Phone Number: ", accounts[index].mobile);
        fgets(temp_ac.mobile, sizeof(temp_ac.mobile), stdin);
        temp_ac.mobile[strcspn(temp_ac.mobile, "\n")] = 0;

        if (check_mobile(temp_ac.mobile)) {
            validnum = 1;
        } else {
            printf("Invalid Phone number");
        }
    } while (!validnum);


    int valid = 0;


    do {
        printf("Old Email: %s. New Email: ", accounts[index].emailaddress);
        fgets(temp_ac.emailaddress, sizeof(temp_ac.emailaddress), stdin);
        temp_ac.emailaddress[strcspn(temp_ac.emailaddress, "\n")] = 0;

        if (is_valid_email(temp_ac.emailaddress)) {
            valid = 1;
        } else {
            printf("Invalid email format! (Example: user@mail.com)\n");
        }
    } while (!valid);
    if(check_to_save()){
        strcpy(accounts[index].name,temp_ac.name);
        strcpy(accounts[index].mobile,temp_ac.mobile);
        strcpy(accounts[index].emailaddress,temp_ac.emailaddress);
    save_data();
    printf("Modified successfully.\n");}
    else
        printf("Account remains the same\n");
}
void change_status() {
    char accNum[20];
    int index = -1;

   do{
    printf("Enter account number: ");
    fgets(accNum,sizeof(accNum),stdin);
    accNum[strcspn(accNum,"\n")]=0;
   }while(!check_ac_number(accNum));

    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].account_no, accNum) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    if (check_to_save()) {
        if (strcmp(accounts[index].status, "active") == 0)
            strcpy(accounts[index].status, "inactive");
        else
            strcpy(accounts[index].status, "active");

        save_data();
        printf("Status changed to: %s\n", accounts[index].status);
    } else {
        printf("Status remains unchanged.\n");
    }
}



void WITHDRAW() {
    char num[20];
    float amount;
    printf("Account Number: ");
    scanf("%s", num);
    getchar();
    int i = -1;
    for(int k=0; k<account_count; k++) if(strcmp(accounts[k].account_no, num)==0) i=k;

    if(i == -1) { printf("Not found.\n"); return; }
    if(strcmp(accounts[i].status, "inactive") == 0) { printf("Account Inactive.\n"); return; }

    char amountStr[50];
    do {
        printf("Amount: ");
        fgets(amountStr, sizeof(amountStr), stdin);
    } while (!check_balance(amountStr));

    amount = atof(amountStr);

    if(amount > 10000) { printf("Max 10k Limit.\n"); return; }
    if(amount > accounts[i].balance) { printf("Insufficient funds.\n"); return; }
    if(!check_daily_limit(num, amount)) return;
    if(check_to_save()){
    accounts[i].balance -= amount;
    log_transaction(num, "Withdraw", amount);
    save_data();
    printf("New Balance: %.2f\n", accounts[i].balance);}
    else
        printf("Withdrawal cancelled. Balance remains unchanged.\n");
}

void DEPOSIT() {
    char num[20];
    float amount;
    printf("Account Number: ");
    scanf("%s", num);
    getchar();
    int i = -1;
    for(int k=0; k<account_count; k++) {
            if(strcmp(accounts[k].account_no, num)==0){
            i=k;
            break;
            }
    }

    if(i == -1) { printf("Not found.\n"); return; }
    if(strcmp(accounts[i].status, "inactive") == 0) { printf("Account Inactive.\n"); return; }

    char amountStr[50];
    do {
        printf("Amount: ");
        fgets(amountStr, sizeof(amountStr), stdin);
    } while (!check_balance(amountStr));

    amount = atof(amountStr);

    if(amount > 10000) { printf("Max 10k Limit.\n"); return; }
    if(check_to_save()){
    accounts[i].balance += amount;
    log_transaction(num, "Deposit", amount);
    save_data();
    printf("New Balance: %.2f\n", accounts[i].balance);}
    else
       printf("Cannot save this transaction at the moment.\n");
}

void TRANSFER() {
    char sNum[20], rNum[20];
    float amount;
    printf("Sender: "); scanf("%s", sNum);
    getchar();
    printf("Receiver: "); scanf("%s", rNum);
    getchar();
    int s = -1, r = -1;
    for(int k=0; k<account_count; k++) {
        if(strcmp(accounts[k].account_no, sNum)==0) s=k;
        if(strcmp(accounts[k].account_no, rNum)==0) r=k;
    }

    if(s == -1 || r == -1) { printf("Account not found.\n"); return; }
    if(strcmp(accounts[s].status,"inactive")==0 || strcmp(accounts[r].status,"inactive")==0) {
        printf("Both must be active.\n"); return;
    }

    char amountStr[50];
    do {
        printf("Amount: ");
        fgets(amountStr, sizeof(amountStr), stdin);
    } while (!check_balance(amountStr));

    amount = atof(amountStr);
    if(amount > accounts[s].balance) { printf("Insufficient funds.\n"); return; }
    if (!check_to_save()) {
        printf("Transfer Cancelled.\n");
        return;
    }
    accounts[s].balance -= amount;
    accounts[r].balance += amount;

    log_transaction(sNum, "Transfer-Out", amount);
    log_transaction(rNum, "Transfer-In", amount);
    save_data();
    printf("Transfer Complete.\n");
}

void REPORT() {
    char num[20], filename[30], line[150];
    printf("Account Number: "); scanf("%s", num);
    sprintf(filename, "%s.txt", num);
    FILE *f = fopen(filename, "r");
    if(!f) { printf("No history.\n"); return; }

    int total = 0;
    while(fgets(line, sizeof(line), f)) total++;
    rewind(f);

    int skip = total - 5;
    if(skip < 0) skip = 0;

    int cur = 0;
    printf("\n--- LAST 5 TRANSACTIONS ---\n");
    while(fgets(line, sizeof(line), f)) {
        if(cur >= skip) printf("%s", line);
        cur++;
    }
    fclose(f);
}

void query() {
    char num[50];
    int i, f = 0;

    do {
        printf("Enter account number: ");
        fgets(num, sizeof(num), stdin);

        num[strcspn(num, "\n")] = '\0';

    } while (!check_ac_number(num));

    for (i = 0; i < account_count; i++) {
        if (strcmp(num, accounts[i].account_no) == 0) {
            f = 1;
            break;
        }
    }

    if (f) {
        printf("********** Account Information ***********\n");
        print_account(accounts[i]);
    } else {
        printf("Account not found\n");
    }
}


void ADVANCED_SEARCH() {
    char keyword[50];
    printf("Enter keyword: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    printf("Search Results: \n");
    int i, f = 0;

    char name_lower[50];
    char keyword_lower[50];

    strcpy(keyword_lower, keyword);
    for(int k = 0; keyword_lower[k]; k++) {
        keyword_lower[k] = tolower(keyword_lower[k]);
    }

    for (i = 0; i < account_count; i++) {
        strcpy(name_lower, accounts[i].name);
        for(int k = 0; name_lower[k]; k++) {
            name_lower[k] = tolower(name_lower[k]);
        }

        if (strstr(name_lower, keyword_lower) != NULL) {
            f = 1;
            print_account(accounts[i]);
            printf("*****************************************\n");
        }
    }

    if (f == 0) {
        printf("No account matches this keyword.\n");
    }
}
    //sorting functions
void SORT_BY_NAME(){
    char ar_of_names[account_count][100];
    int printed[account_count];
    int i,j;
    for(int i=0;i<account_count;i++){
    strcpy(ar_of_names[i] ,accounts[i].name);
        printed[i]=0;

    }
    for(i=0;i<account_count-1;i++){
        for(j=0;j<account_count-1;j++){
                if(strcmp(ar_of_names[j], ar_of_names[j+1]) > 0){
                char temp[100];
                strcpy(temp, ar_of_names[j]);
                strcpy(ar_of_names[j], ar_of_names[j+1]);
                strcpy(ar_of_names[j+1], temp);

        }
    }}
    for(i=0;i<account_count;i++){
        for(j=0;j<account_count;j++){
            if(!printed[j] && strcmp(ar_of_names[i],accounts[j].name)==0){
                print_account(accounts[j]);
                printf("*******************************************\n");
                printed[j]=1;
                break;
            }
        }
    }

}
void SORT_BY_BALANCE(){
     float ar_of_balance[account_count];
     int printed[account_count];
     int i,j;
     for(i=0;i<account_count;i++){
        ar_of_balance[i]= accounts[i].balance;
        printed[i]=0;
     }

     for(i=0;i<account_count-1;i++){
            for(j=0;j<account_count-1;j++){
                if(ar_of_balance[j]<ar_of_balance[j+1]){
                    float temp = ar_of_balance[j];
                    ar_of_balance[j]=ar_of_balance[j+1];
                    ar_of_balance[j+1]=temp;
                }
            }

     }
     for(i=0;i<account_count;i++){
        for(j=0;j<account_count;j++){
            if(ar_of_balance[i]==accounts[j].balance && !printed[j]){
                print_account(accounts[j]);
                printed[j] =1;
                printf("********************************************\n");
                break;
            }
        }
     }

}
void SORT_BY_DATE() {
    int printed[account_count];
    for(int i = 0; i < account_count; i++) printed[i] = 0;

    for(int j = 0; j < account_count; j++){
        int latest = -1;

        for(int i = 0; i < account_count; i++){
            if(printed[i]) continue;

            if(latest == -1) {
                latest = i;
            }
            else {
                if(accounts[i].dateopened.year > accounts[latest].dateopened.year ||
                   (accounts[i].dateopened.year == accounts[latest].dateopened.year &&
                    accounts[i].dateopened.month > accounts[latest].dateopened.month)) {
                    latest = i;
                }
            }
        }

        print_account(accounts[latest]);
        printf("**************************************************\n");
        printed[latest] = 1;
    }
}
void SORT_BY_STATUS(){
    int i;
    for(i=0;i<account_count;i++){
        if (strcmp(accounts[i].status,"active")== 0)
            {print_account(accounts[i]);
            printf("******************************\n");}
    }
    for(i=0;i<account_count;i++){
        if(strcmp(accounts[i].status,"inactive")==0)
            {print_account(accounts[i]);
            printf("******************************\n");}
    }
}

//delete functions
void DELETE_INACTIVE_AC(){
     int curr_year,curr_month,new_count=account_count,i,j,deleted_ac=0;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    curr_year  = t->tm_year + 1900;
    curr_month = t->tm_mon + 1;
    account temp[account_count];
    for(i=0;i<account_count;i++){
        temp[i]=accounts[i];
    }


    for(i=0;i<new_count;i++){
            int months_diff = (curr_year * 12 + curr_month) -
                  (temp[i].dateopened.year * 12 + temp[i].dateopened.month);
        if (strcmp(temp[i].status, "inactive") == 0 &&temp[i].balance == 0 && months_diff >= 3){
                for (j = i; j < new_count - 1; j++) {
                temp[j] = temp[j + 1];
            }

            new_count--;
            deleted_ac++;
            i--;
        }}

        if (deleted_ac == 0) {
       printf("No inactive accounts older than 3 months with zero balance found.\n");
        return;
    }

    else if (check_to_save()) {

        for (i = 0; i < new_count; i++) {
            accounts[i] = temp[i];
        }
        account_count = new_count;
        save_data();
        printf("Deletion completed successfully.\n");
    }
    else {
        printf("Deletion canceled.\n");
    }
}
void DELETE_MULTIBLE(){
    int choice,f=0;
    do{printf("Choose how to delete the accounts 1.By date  2.Inactive accounts \n");
    printf("Enter choice : ");
    scanf("%d",&choice);
    getchar();
    switch (choice){
    case 1:
        f=1;
        DELETE_BY_DATE();
        break;
    case 2:
        f=1;
        DELETE_INACTIVE_AC();
        break;
    default:
        printf("Invalid input!try again. \n");
    }
    }while(!f);

}
void DELETE_BY_DATE() {
    char date[10];
    int year, month, i, j;
    int deleted_ac = 0;
    int new_count = account_count;

    printf("Enter the date (MM-YYYY): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';

    char *token = strtok(date, "-");
    if (token == NULL) {
        printf("Invalid format\n");
        return;
    }
    month = atoi(token);

    token = strtok(NULL, "-");
    if (token == NULL) {
        printf("Invalid format\n");
        return;
    }
    year = atoi(token);

    if (month < 1 || month > 12) {
        printf("Invalid month\n");
        return;
    }

    account temp[account_count];


    for (i = 0; i < account_count; i++) {
        temp[i] = accounts[i];
    }


    for (i = 0; i < new_count; i++) {
        if (temp[i].dateopened.year == year &&
            temp[i].dateopened.month == month) {

            for (j = i; j < new_count - 1; j++) {
                temp[j] = temp[j + 1];
            }

            new_count--;
            deleted_ac++;
            i--;
        }
    }

    if (deleted_ac == 0) {
        printf("No account created in this date.\n");
        return;
    }



    else if (check_to_save()) {

        for (i = 0; i < new_count; i++) {
            accounts[i] = temp[i];
        }
        account_count = new_count;
        save_data();
        printf("Deletion completed successfully.\n");
    }
    else {
        printf("Deletion canceled.\n");
    }
}
void PRINT(){
        int choice,f=0;
        do{printf("sort accounts by 1.Name , 2.Balance , 3.Date opened 4.Status\n");
            printf("Enter choice: ");
            scanf("%d",&choice);

        switch (choice){
        case 1:
            f=1;
            SORT_BY_NAME();
                    break;
        case 2:
            f=1;
            SORT_BY_BALANCE();
            break;
        case 3:
            f=1;
            SORT_BY_DATE();
            break;
        case 4:
            f=1;
            SORT_BY_STATUS();
           break;
        default:
            printf("invalid choice!try again.\n");
        }}while(!f);
        }
//******helper functions*******
// Validates email: Must contain '@', a '.' after '@', and no spaces.
int is_valid_email(char *email) {
    int len = strlen(email);
    char *at = strchr(email, '@');
    char *dot = strrchr(email, '.');

    if (len < 5) return 0;
    if (!at) return 0;
    if (at == email) return 0;
    if (!dot) return 0;
    if (dot <= at) return 0;
    if (dot - at == 1) return 0;

    if (dot == (email + len - 1)) return 0;
    for(int i = 0; i < len; i++) {
        if(email[i] == ' ') return 0;
    }

    return 1; // Email is valid
}

void menu() {
    int choice;

    while(1) {

        printf("\n1.ADD\n2.DELETE\n3.MODIFY\n4.SEARCH\n5.ADV.SEARCH\n6.STATUS\n");
        printf("7.WITHDRAW\n8.DEPOSIT\n9.TRANSFER\n10.REPORT\n11.PRINT\n12.DELETE MULTIPLE\n13.QUIT\n");
        printf("Select: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: ADD(); break;
            case 2: delete_account(); break;
            case 3: modify(); break;
            case 4: query(); break;
            case 5: ADVANCED_SEARCH(); break;
            case 6: change_status(); break;
            case 7: WITHDRAW(); break;
            case 8: DEPOSIT(); break;
            case 9: TRANSFER(); break;
            case 10: REPORT(); break;
            case 11: PRINT(); break;
            case 12: DELETE_MULTIBLE(); break;
            case 13: quit(); break;
            default: printf("Invalid.\n");
        }
        int Secondchoice;
            printf("\nOperation finished.\n");
            printf("1. Go back to Menu\n");
            printf("2. Exit Program\n");
            printf("Select: ");
            scanf("%d", &Secondchoice);
            getchar();
            if (Secondchoice == 2) {
                printf("Exiting program...\n");
                exit(0);
            }
    }

}
void format_name(char *name) {
    int i = 0, new_word = 1, j = 0;
    char temp[100];

    while (name[i]) {
        if (isspace(name[i]) || name[i] == ',') {
            if (j > 0 && temp[j - 1] != ' ') temp[j++] = ' ';
            new_word = 1;
        } else {
            temp[j++] = new_word ? toupper(name[i]) : tolower(name[i]);
            new_word = 0;
        }
        i++;
    }

    if (j > 0 && temp[j - 1] == ' ') j--;

    temp[j] = 0;
    strcpy(name, temp);
}
int check_unique(char* num) {
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].account_no, num) == 0) {
            printf("Account number already exists.\n");
            return 0;
        }
    }
    return 1;
}

int check_ac_number(char *num) {
    if (strlen(num) != 10) { printf("Must be 10 digits.\n"); return 0; }
    for (int i = 0; i < 10; i++) if (!isdigit(num[i])) { printf("Digits only.\n"); return 0; }
    return 1;
}

    void print_account(account ac){
            printf("Account number: %s\n", ac.account_no);
            printf("Name: %s\n", ac.name);
            printf("Email: %s\n", ac.emailaddress);
            printf("Balance: %.2f $\n", ac.balance);
            printf("Mobile: %s\n", ac.mobile);
            printf("Date Opened: %s-%d\n", months[ac.dateopened.month - 1], ac.dateopened.year);
            printf("Status: %s\n", ac.status);
            }

void log_transaction(char *ac_num, char *type, float amount) {
    char filename[30];
    sprintf(filename, "%s.txt", ac_num);
    FILE *f = fopen(filename, "a");
    if(f) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(f, "%d-%02d-%02d,%s,%.2f\n", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, type, amount);
        fclose(f);
    }
}

int check_daily_limit(char *ac_num, float new_amount) {
    char filename[30];
    sprintf(filename, "%s.txt", ac_num);
    FILE *f = fopen(filename, "r");
    if(!f) return 1;

    char line[100];
    float total = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char today[15];
    sprintf(today, "%d-%02d-%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday);

    while(fgets(line, sizeof(line), f)) {
        char *d = strtok(line, ",");
        char *typ = strtok(NULL, ",");
        char *amt = strtok(NULL, "\n");
        if(d && typ && amt) {
            if(strcmp(d, today) == 0 && strcmp(typ, "Withdraw") == 0) total += atof(amt);
        }
    }
    fclose(f);

    if(total + new_amount > 50000) {
        printf("Daily Limit Exceeded (Used: %.2f)\n", total);
        return 0;
    }
    return 1;
}
//check balance validity
int check_balance(char*balance){
int i,n=strlen(balance),dot=0;

    if (n > 0 && balance[n - 1] == '\n') {
        balance[n - 1] = '\0';
        n--;
    }

if (n == 0)
        return 0;
if(balance[0]=='-'){
    printf("enter positve number:\n");
    return 0;
}
    for (i = 0; i < n; i++) {
        if (balance[i] == '.') {
            dot++;
            if (dot > 1){
                    printf("Invalid Input\n");
                return 0;}
        }
        else if (!isdigit(balance[i])) {
            printf("Invalid Input\n");
            return 0;
        }
    }
    return 1;
}
int check_mobile(char* mobile){
int n=strlen(mobile),i;
if(n > 0 && mobile[n - 1] == '\n') {
    mobile[n - 1] = '\0';
    n--;
}
if(n!=11){
    printf("Must be 11 digit\n");
    return 0;
}
for(i=0;i<n;i++){
    if(!isdigit(mobile[i])){
            printf("Invalid input\n");
            return 0;

    }
}
return 1;
}
int check_to_save(){
    int answer;
    printf("Do you want to save the changes\n");
    printf("1. Yes  2.No\n");
    printf("Enter your answer: ");
    scanf("%d",&answer);
    getchar();
    if(answer ==1 ){
        return 1;
    }
    else if(answer==2){
        return 0;
    }
    else {printf("invalid input");
    return 0;
    }

}

int check_name(char *name) {
    int i, len = strlen(name);
    int has_letter = 0;

    if (len > 0 && name[len - 1] == '\n') {
        name[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
            printf("Invalid Name\n");
            return 0;} // Empty input

    for (i = 0; i < len; i++) {
        if (name[i] == ' ') {
            continue;
        }
        if (isdigit(name[i])) {
            printf("Invalid Name: Numbers are not allowed.\n");
            return 0;
        }

        if (!isalpha(name[i])) {
            printf("Invalid Name: Symbols are not allowed.\n");
            return 0;
        }
        has_letter = 1;
    }

    if (!has_letter) {
        printf("Invalid Name: Must contain letters.\n");
        return 0;
    }

    return 1;
}
