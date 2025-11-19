#include "header.h"

struct accounts {
    char code[6];
    char name[15];
    char password[10];
    char country[20];
    int limitofwithdraw;
    int limitofdeposit;
    int limitoftransfer;
    int balance;
    char flag[10];
}clientData;

int client() {
    int choice;
    char userpassword[10];
    char usercode[10];
    int check,c;
    while((c=getchar()), c != '\n' && c != EOF);
    while (true)
    {
        system("cls");
        printf("Welcome to ROHYAS BANK\nPlease input your code: ");
        fgets(usercode, sizeof(usercode), stdin);
        usercode[strcspn(usercode, "\r\n")] = '\0'; 

        printf("Enter your password: ");
        fgets(userpassword, sizeof(userpassword), stdin);
        userpassword[strcspn(userpassword, "\r\n")] = '\0'; 

        check = verifying(usercode, userpassword);

        if (check == 0) {
            int back=clientMenu();
            if(!back) return 0;
        } else if (check==2) {
            printf("\nYour Account is blocked.\nPlease contact administration for further details.\n");
            Sleep(5000);
        } else {
            printf("Invalid code or password\n");
            Sleep(5000);
        }
    }
    return 0;
}
int verifying(char usercode[10], char userpassword[10]){

    char code[5], name[50], country[20],flag[10]="green",pass[10];
    int lowpd,lodpd,lot,balance;

    FILE* fptr = fopen("accounts.txt", "r");
    if (fptr == NULL){
        printf("Error in opening file\n");
        return 1; 
    }

    char line[256];

    while (fgets(line, sizeof(line), fptr) != NULL){
        line[strcspn(line,"\r\n")]='\0';
        sscanf(line, "%4[^,],%14[^,],%9[^,],%15[^,],%d,%d,%d,%d,%5[^,]", code, name, pass,country,&lowpd,&lodpd,&lot,&balance,flag);
        flag[strcspn(flag,"\r\n")]='\0';

        if (strcmp(code, usercode) == 0 &&
            strcmp(pass, userpassword) == 0) {
            if(strcmp(flag,"red")==0) return 2;
            strncpy(clientData.code,code,sizeof(clientData.code)-1);
            clientData.code[sizeof(clientData.code)-1]='\0';
            strncpy(clientData.name ,name,sizeof(clientData.name)-1);
            clientData.name[sizeof(clientData.name)-1]='\0';
            strncpy(clientData.password,pass,sizeof(clientData.password)-1);
            clientData.password[sizeof(clientData.password)-1]='\0';
            strncpy(clientData.country,country,sizeof(clientData.country)-1);
            clientData.country[sizeof(clientData.country)-1]='\0';
            clientData.limitofwithdraw=lowpd;
            clientData.limitofdeposit=lodpd;
            clientData.limitoftransfer=lot;
            clientData.balance=balance;
            strcpy(clientData.flag,flag);
            
            fclose(fptr);
            return 0; 
        }
    }

    fclose(fptr);
    return 1; 
}

int clientMenu(){
    int chose,c;
    while(1){
        system("cls");
        printf("** WELCOME TO ROHYAS BANK **;\nClient %s\n",clientData.name);
        printf("\n1. Withdraw\n2. Deposit\n3. Money Transfer\n4. Check Balance\n5. Acoount Info\n6. Get Transaction History\n7. Freeze Account\n8. Exit\nChose: ");
        scanf("%d",&chose);
        switch(chose){
            case 1:
                withdrawn();
                break;
            case 2:
                deposit();
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                return 0;
                break;
            default:
                printf("Invalid Input");
                Sleep(2000);
                continue;
                break;
        }
    }
    return 0; 
}

int withdrawn(){
    int withdrawnAmount;
    system("cls");
    printf("=== WITHDRAW MONEY ===\n");
    printf("Enter the amount to be withdrawn: ");
    if(scanf("%d",&withdrawnAmount) != 1 || withdrawnAmount<=0){
        printf("\nInvalid amount entered!\n");
        printf("Returning to menu in 3 seconds...\n");
        Sleep(3000);
        return 0;
    }

    if(withdrawnAmount>clientData.balance){
        system("cls");
        printf("Unsufficient balance\n");
        printf("Returning to Menu page in 5 seconds...");
        Sleep(5000);
        return 0;
    }

    if(withdrawnAmount>clientData.limitofwithdraw || !withdrawnLimitCross(withdrawnAmount) || !checkLastTransactions()){
        system("cls");
        printf("\nYour account %s has been blocked, Due to suspicius activity\n\tExiting in 10 seconds");
        Sleep(10000);
        flagToRed();
        return 2;
    }

    clientData.balance -= withdrawnAmount;


    time_t t;
    struct tm *timeinfo;
    char date[20],currentTime[20];
    time(&t);
    timeinfo=localtime(&t);
    strftime(date,sizeof(date),"%y-%m-%d",timeinfo);
    strftime(currentTime,sizeof(currentTime),"%H:%M:%S",timeinfo);
    FILE *pFile=fopen("transactions.txt","a");
    fprintf(pFile,"%s,w,%d,%s,%s\n",clientData.code,withdrawnAmount,date,currentTime);
    fclose(pFile);

    updateUser();
    printf("\nWithdrawn was successfull\nReturning to menu in 5 seconds");

    Sleep(5000);

    return 1;
}

int checkLastTransactions(){
    int tractOfCloseTimes=0;
    char tempText[1024];
    char eachUseriId[5];
    char fetchDate[20];
    char fetchTime[20],method;

    FILE *pFile = fopen("transactions.txt","r");
    time_t t;
    struct tm *timeinfo;
    char currentDate[20],currentTime[20];
    time(&t);
    timeinfo = localtime(&t);
    strftime(currentDate, sizeof(currentDate), "%y-%m-%d", timeinfo);
    strftime(currentTime,sizeof(currentTime),"%H:%M:%S",timeinfo);

    while(fgets(tempText,sizeof(tempText),pFile)){
        sscanf(tempText,"%[^,],%c,%*[^,],%[^,],%[^,]",eachUseriId,&method,fetchDate,fetchTime);
        if(strcmp(eachUseriId,clientData.code)==0 && method=='w'){
            int yy,mm,dd,hh,min,ss;
            struct tm file_tm={0};

            sscanf(fetchDate,"%d-%d-%d",&yy,&mm,&dd);
            sscanf(fetchTime,"%d:%d:%d",&hh,&min,&ss);

            file_tm.tm_year=yy+100;
            file_tm.tm_mon=mm-1;
            file_tm.tm_mday=dd;
            file_tm.tm_hour=hh;
            file_tm.tm_min=min;
            file_tm.tm_sec=ss;

            time_t file_time=mktime(&file_tm);
            float diff_seconds=difftime(t,file_time);
            if(diff_seconds<3600.0){
                tractOfCloseTimes+=1;
            }
        }
    }
    fclose(pFile);
    if(tractOfCloseTimes==3){
        return 0;
    }
    return 1;
}

int withdrawnLimitCross(int withdrawnAmount){
    char tempText[1024];
    char eachUseriId[5];
    char fetchDate[20], method,fetchTime[20];
    int fetchWithdrawns = 0;
    int sumFetchWithdrawns = 0;

    FILE *pFile = fopen("transactions.txt","r");
    if (!pFile) return 1;

    time_t t;
    struct tm *timeinfo;
    char date[20];
    time(&t);
    timeinfo = localtime(&t);
    strftime(date, sizeof(date), "%y-%m-%d", timeinfo);

    while(fgets(tempText,sizeof(tempText),pFile)){
        tempText[strcspn(tempText, "\r\n")] = '\0';
        
        sscanf(tempText,"%4[^,],%c,%d,%19[^,],%19[^,]",eachUseriId,&method,&fetchWithdrawns,fetchDate,fetchTime);

        if(strcmp(eachUseriId,clientData.code)==0 && strcmp(fetchDate,date)==0 && method=='w'){
            sumFetchWithdrawns += fetchWithdrawns;
        }
    }
    fclose(pFile);

    if((sumFetchWithdrawns + withdrawnAmount) > clientData.limitofwithdraw){ 
        printf("%d",(sumFetchWithdrawns + withdrawnAmount));
        return 0;
    }
    return 1;
}
void flagToRed(){
    strcpy(clientData.flag,"red");
    updateUser();
}

int deposit(){
    int depositAmount;
    system("cls");
    printf("=== WITHDRAW MONEY ===\n");
    printf("Enter the amount you want to deposit: ");
    if(scanf("%d",&depositAmount) != 1 || depositAmount<=0){
        printf("\nInvalid amount entered!\n");
        printf("Returning to menu in 3 seconds...\n");
        Sleep(3000);
        return 0;
    }
    if((depositAmount+clientData.balance)<=clientData.limitofdeposit){
        clientData.balance+=depositAmount;
        time_t t;
        struct tm *timeinfo;
        char date[20],currentTime[20];
        time(&t);
        timeinfo=localtime(&t);
        strftime(date,sizeof(date),"%y-%m-%d",timeinfo);
        strftime(currentTime,sizeof(currentTime),"%H:%M:%S",timeinfo);
        FILE *pFile=fopen("transactions.txt","a");
        fprintf(pFile,"%s,d,%d,%s,%s\n",clientData.code,depositAmount,date,currentTime);
        fclose(pFile);  
        updateUser();

        return 1;
    }else{
        system("cls");
        printf("\tThis deposit violate our bank policy.\nYour bank account holding limit is %d, your balance is %d, you want to deposit %d which exceeds bank account holding limit.\n\tWe hope you can understand\t\n",clientData.limitofdeposit,clientData.balance,depositAmount);
        printf("Returning to welcome page in 5 seconds...");
        Sleep(5000);
        return 0;
    }

}
// int transfer(){
//     char code[5];
//     int transferAmount;
//     while(1){
//         system("cls");
//         printf("=== Transfer MONEY ===\n");
//         printf("Transfer to? ")
//         printf("Transfer to: ")
//         scanf("%s",code);
//     }
// }
void updateUser(){
    char tempText[1024];
    char eachUseriId[5];
    FILE *pFile=fopen("accounts.txt","r");
    FILE *pTempFile=fopen("tempFile.txt","w");
    while(fgets(tempText,sizeof(tempText),pFile)){
        sscanf(tempText,"%[^,]",eachUseriId);
        if(strcmp(eachUseriId,clientData.code)==0){
            fprintf(pTempFile,"%s,%s,%s,%s,%d,%d,%d,%d,%s\n",clientData.code,clientData.name,clientData.password,clientData.country,clientData.limitofwithdraw,clientData.limitofdeposit,clientData.limitoftransfer,clientData.balance,clientData.flag);
        }else{
            fputs(tempText,pTempFile);
        }
    }
    fclose(pFile);
    fclose(pTempFile);

    remove("accounts.txt");
    rename("tempFile.txt","accounts.txt");
}
