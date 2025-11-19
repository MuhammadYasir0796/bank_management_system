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
                transfer();
                break;
            case 4:
                checkuserbalance();
                break;
            case 5:
                viewuseraccountinfo();
                break;
            case 6:
                viewtransactionhistory();
                break;
            case 7:
                if(!freezeaccount()) return 0;
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
//trannsfer feature function
int transfer(){
    char code[5],name[20],country[20],pass[9],flag[10];
    int transferAmount,c;
    int wl,dl,tl,balance;
    while(1){
        //setup
        system("cls");
        printf("=== Transfer MONEY ===\n");
        printf("Write 0 or 0 to return to menu\n\n");
        printf("To whom you want to transfer? Code: ");
        scanf("%4s",code);
        while((c=getchar()), c != '\n' && c != EOF);     //buffer clearing
        printf("Amount of transfer: ");
        if(scanf("%d",&transferAmount)!=1 || transferAmount<0){
            printf("\nInvalid input\n Wait for 3 seconds.\n");
            Sleep(3000);
            continue;
        }
        while((c=getchar()), c != '\n' && c != EOF);    //buffer clearing
        //Checking 0 0
        if(strcmp(code,"0")==0 || transferAmount==0) return 0;
        //check if user is transfering to itself
        if(strcmp(code,clientData.code)==0){
            printf("You can not transfer to yourself");
            Sleep(3000);
            continue;
        }
        //check if account exist
        int otherAccountLimit=transferCheckAccountExits(code,name,pass,country,&wl,&dl,&tl,&balance,flag);
        if(!otherAccountLimit){
            printf("\nThe %s account does not exits.\nPlease double check the code.\n\tReturning in 5 seconds...",code);
            Sleep(5000);
            continue;
        }
        //check if other account is blocked
        if(strcmp(flag,"red")==0){
            printf("\nYou can not transfer to %s account, because it is blocled.\n",code);
            Sleep(5000);
            continue;
        }
        //checking if the other account eceed its account limit
        if((balance+transferAmount)>dl){
            printf("\nYou can not transfer to %s account, because it will exceed that accounts limit.\n",code);
            Sleep(5000);
            continue;
        }
        //checking if current account transfer limit doeos not exceed
        if(transferAmount>clientData.limitoftransfer){
            printf("\nYour account transfer limit is %d and you are transfering %d, \n\tthat is exceeding account transfer limit\n",clientData.limitoftransfer,transferAmount);
            Sleep(5000);
            continue;
        }
        //updating blance struct and updating current account info in accounts.txt
        clientData.balance -= transferAmount;
        updateUser();

        //adding transaction info to transaction.txt
        time_t t;
        struct tm *timeinfo;
        char date[20],currentTime[20];
        time(&t);
        timeinfo=localtime(&t);
        strftime(date,sizeof(date),"%y-%m-%d",timeinfo);
        strftime(currentTime,sizeof(currentTime),"%H:%M:%S",timeinfo);
        FILE *pFile=fopen("transactions.txt","a");
        fprintf(pFile,"%s,t,%d,%s,%s,%s\n",clientData.code,transferAmount,date,currentTime,code);
        fclose(pFile); 

        //updating other accounts 
        balance += transferAmount;
        char tempText[1024];
        char eachUseriId[5];
        FILE *pFileO=fopen("accounts.txt","r");
        FILE *pTempFile=fopen("tempFile.txt","w");
        while(fgets(tempText,sizeof(tempText),pFileO)){
            sscanf(tempText,"%4[^,]",eachUseriId);
            if(strcmp(eachUseriId,code)==0){
                fprintf(pTempFile,"%s,%s,%s,%s,%d,%d,%d,%d,%s\n",code,name,pass,country,wl,dl,tl,balance,flag);
            }else{
                fputs(tempText,pTempFile);
            }
        }
        fclose(pFileO);
        fclose(pTempFile);

        remove("accounts.txt");
        rename("tempFile.txt","accounts.txt");

        break;
    }
    printf("\nSuccessfully Tranfered money to %s",code);
    Sleep(3000);
    return 0;
}
int transferCheckAccountExits(char code[],char *n,char *p,char *c,int *wl,int *dl,int *tl, int *b,char *f){
    char line[1024],fetchCode[5],fetchName[20],fetchPass[9],fetchC[20],fetchFlag[10];
    int wlimit,dlimit,tlimit,balance;
    FILE *pFile=fopen("accounts.txt","r");
    while(fgets(line,sizeof(line),pFile)){
        sscanf(line,"%4[^,],%19[^,],%8[^,],%19[^,],%d,%d,%d,%d,%9s",fetchCode,fetchName,fetchPass,fetchC,&wlimit,&dlimit,&tlimit,&balance,fetchFlag);
        if(strcmp(code,fetchCode)==0){
            strcpy(n, fetchName);
            strcpy(p, fetchPass);
            strcpy(c, fetchC);
            *wl=wlimit;
            *dl=dlimit;
            *tl=tlimit;
            *b=balance;
            strcpy(f,fetchFlag);
            fclose(pFile);
            return 1;
        }
    }  
    fclose(pFile);
    return 0;
}
void updateUser(){
    char tempText[1024];
    char eachUseriId[5];
    FILE *pFile=fopen("accounts.txt","r");
    FILE *pTempFile=fopen("tempFile.txt","w");
    while(fgets(tempText,sizeof(tempText),pFile)){
        sscanf(tempText,"%4[^,]",eachUseriId);
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
void checkuserbalance(){
    system("cls");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
  //  printf("checking your balance please wait\n");
    printf("Your current balance is: ");
    printf("%d", clientData.balance);
    printf("\n\nPlease press enter to exit\n");
    getchar();
}

void viewuseraccountinfo(){
    system("cls");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("====================================\n");
    printf("           ACCOUNT DETAILS          \n");
    printf("====================================\n\n");

    printf("Account Holder : %s\n",  clientData.name);
    printf("Account Number : %s\n",  clientData.code);
    printf("Country        : %s\n",  clientData.country);
    printf("Status         : %s\n",  clientData.flag);
    printf("\n");

    printf("------ Financial Information ------\n");
    printf("Current Balance        : %d\n", clientData.balance);
    printf("Withdraw Limit (Daily) : %d\n", clientData.limitofwithdraw);
    printf("Deposit Limit (Total)  : %d\n", clientData.limitofdeposit);
    printf("Transfer Limit (Daily) : %d\n", clientData.limitoftransfer);
    printf("====================================\n\n");

    printf("Press ENTER to return to menu...");
    getchar();
}
     
int freezeaccount(){
    char confirmedcode[9];
    system("cls");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("It seems you want to freeze your account, \nPlease input your code again: ");
    fgets(confirmedcode,sizeof(confirmedcode),stdin);
    confirmedcode[strcspn(confirmedcode, "\n")] = '\0';

    if (strcmp(confirmedcode, clientData.password) != 0){
        printf("\nInvalid\nReturning to menu.\n");
        Sleep(5000);
        return 0;
    }
    else{
        flagToRed();
        printf("Account frozen successfully\n");
        printf("Please press enter to continue");
        while ((c = getchar()) != '\n' && c != EOF);
        int ch=getchar();
        return 0;
    }
    return 0;
}

void viewtransactionhistory() {
    system("cls");

    char tempcode[6];
    char event[2];
    char amount[10];
    char date[15];
    char time[15];
    char buffer[1024];
    int c;
    
    while ((c = getchar()) != '\n' && c != EOF);  // clear input buffer
    printf("Now accessing user transaction history:\n");

    FILE* pfile = fopen("transactions.txt", "r");
    if (pfile == NULL) {
        printf("Error accessing the transactions history file\n");
        return;
    }

    int found = 0;

    while (fgets(buffer, sizeof(buffer), pfile) != NULL) {
        // parse the line: code,type,amount,date,time
        sscanf(buffer, "%5[^,],%1[^,],%9[^,],%14[^,],%14[^\n]", tempcode, event, amount, date, time);

        if (strcmp(tempcode, clientData.code) == 0) {
            printf("Code: %s, Type: %s, Amount: %s, Date: %s, Time: %s\n",
                   tempcode, event, amount, date, time);
            found = 1;
        }
    }

    if (found == 0) {
        printf("No transaction history detected\n");
    }

    fclose(pfile);

    printf("Press Enter to continue: ");
    getchar();
}