#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//admin
int admin();
int adminLogin();  
int createClient();
int checkUniqency(char *id);
char* generateCode();
int flagChange();
int updateAccount();
void viewAccounts();


//Client
int client();
int verifying(char usercode[10], char userpassword[10]);
// void viewdetails();
void checkstatus();
int clientMenu();
int withdrawn();
int withdrawnLimitCross(int withdrawnAmount);
int checkLastTransactions();
void flagToRed();
void updateUser();
int deposit();
int transfer()

#endif
