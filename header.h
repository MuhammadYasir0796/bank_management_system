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

#endif
