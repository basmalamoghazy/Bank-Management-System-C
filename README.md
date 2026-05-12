# Bank Management System in C

A console-based **Bank Management System** built using the C programming language.
This project allows users to manage bank accounts with features like account creation, deposits, withdrawals, transfers, account searching, sorting, reporting, authentication, and transaction history tracking.

---

## Features

### Authentication System

* Secure login system using username and password
* Maximum of 3 login attempts
* Automatic creation of default admin credentials if no user file exists

Default credentials:

```txt
Username: admin
Password: 123
```

---

## Account Management

* Add new bank accounts
* Modify account information
* Delete accounts
* Activate / deactivate accounts
* Search accounts by account number
* Advanced search using customer name

---

## Banking Operations

* Deposit money
* Withdraw money
* Transfer money between accounts
* Daily withdrawal limit checking
* Transaction logging system

---

## Reports & Printing

* Display last 5 account transactions
* Print accounts sorted by:

  * Name
  * Balance
  * Date opened
  * Status

---

## Validation System

The system validates:

* Account number format
* Unique account numbers
* Email addresses
* Mobile numbers
* Account balance input
* Customer names

---

## File Handling

The project uses file handling for permanent data storage.

### Files Used

| File                   | Purpose                                     |
| ---------------------- | ------------------------------------------- |
| `accounts.txt`         | Stores all account information              |
| `users.txt`            | Stores login credentials                    |
| `<account_number>.txt` | Stores transaction history for each account |

---

## Technologies Used

* C Programming Language
* Dynamic Memory Allocation (`malloc`, `realloc`)
* File Handling
* Structures
* Arrays
* String Manipulation
* Time & Date Functions

---

## Project Structure

```txt
├── main.c
├── accounts.txt
├── users.txt
├── README.md
```

---

## Account Structure

```c
typedef struct {
    char account_no[20];
    char name[50];
    char mobile[20];
    char emailaddress[60];
    float balance;
    Date dateopened;
    char status[14];
} account;
```

---

## Main Menu

```txt
1.ADD
2.DELETE
3.MODIFY
4.SEARCH
5.ADV.SEARCH
6.STATUS
7.WITHDRAW
8.DEPOSIT
9.TRANSFER
10.REPORT
11.PRINT
12.DELETE MULTIPLE
13.QUIT
```

---

## How to Run

### Using GCC

Compile the program:

```bash
gcc main.c -o bank
```

Run the program:

```bash
./bank
```

---

## Sample Functionalities

### Add Account

* Creates a new bank account
* Automatically sets opening date
* Default status is `active`

### Withdraw

* Checks:

  * Account existence
  * Account status
  * Daily limit
  * Sufficient balance

### Transfer

* Transfers money between two active accounts
* Logs both transfer-in and transfer-out transactions

---

## Security Features

* Login authentication
* Daily withdrawal limit
* Inactive account protection
* Confirmation before saving changes

---

## Future Improvements

* Admin panel
* Password encryption
* GUI version
* Database integration
* Multi-user support
* Interest calculation
* Loan management system

---

## Learning Outcomes

This project helped practice:

* Modular programming in C
* File processing
* Dynamic memory management
* Data validation
* Searching and sorting algorithms
* Structs and pointers
* Real-world system simulation

---

## Authors

Developed as a Banking System project using C programming language.
