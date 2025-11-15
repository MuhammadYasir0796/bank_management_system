#include "header.h"

int admin(){
    int isLoginSuccess=adminLogin();
    if(isLoginSuccess != 2){
        printf("\nInvalid Credentials\n");
        printf("\nLeaving in 5 seconds");
        Sleep(5000);
        return 0;
    }
    system("cls");
    int chose,c;
    printf("** Welcome to ROHYAS BANK Admin Portal **\n");
    printf("1. Create a Client Account\n2. Block and Unblock Accounts\n3. Update an client Account\n4. View Accounts\n5. Exit\nChose: ");
    while(scanf("%d",&chose) != 1){
                printf("\nInvalid Input\n");
                while((c=getchar()), c != '\n' && c != EOF);
            }
    switch (chose)
    {
    case 1:
        createClient();
        break;
    case 2:
        flagChange();
        break;
    case 3:
        updateAccount();
        break;
    case 4:
        viewAccounts();
        break;
    case 5:
        return 0;
        break;
    default:
        printf("\nInvalid Input\n");
        break;
    }
    return 0;
}

int adminLogin(){
    int c;
    while((c=getchar()), c != '\n' && c != EOF);
    system("cls");

    char pin[5]="1111",pass[9]="12345678";
    char userPin[6], userPass[10];  
    int sizeUserPin,sizeUserPass;

    printf("Code: ");
    fgets(userPin, sizeof(userPin), stdin);
    userPin[strcspn(userPin, "\r\n")] = '\0';
    printf("Pin: ");
    fgets(userPass, sizeof(userPass), stdin);
    userPass[strcspn(userPass, "\r\n")] = '\0';

    sizeUserPin=strlen(userPin);
    sizeUserPass=strlen(userPass);

    if(sizeUserPin != 4 || sizeUserPass != 8) return 0;
    if(strcmp(userPin,pin)!=0 || strcmp(userPass,pass)!=0) return 1;

    return 2;
}

int createClient(){
    int c;
    char name[50], country[20],flag[10]="green",pass[10];
    int lowpd,lodpd,lot,balance;
    system("cls");
    printf("** Creating Client Account **\n");
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\r\n")] = '\0';

    while(true){
        printf("Password: ");
        fgets(pass, sizeof(pass), stdin);
        pass[strcspn(pass, "\r\n")] = '\0';
        if(strlen(pass) != 8) continue;
        else break;
    }

    printf("Country: ");
    fgets(country, sizeof(country), stdin);
    country[strcspn(country, "\r\n")] = '\0';
    printf("Limit of withdrawn per day: ");
    scanf("%d",&lowpd);
    printf("Limit of deposit per day: ");
    scanf("%d",&lodpd);
    printf("Limit of transfer: ");
    scanf("%d",&lot);
    printf("Initial deposit: ");
    scanf("%d",&balance);

    char *code=generateCode();

    FILE *pFile=fopen("accounts.txt","a");
    fprintf(pFile,"%s,%s,%s,%s,%d,%d,%d,%d,green\n",code,name,pass,country,lowpd,lodpd,lot,balance);
    printf("\nUser created Successfully\nCode: %s\nPassword: %s",code,pass);
    Sleep(5000);
    fclose(pFile);
    return 0;
}
int checkUniqency(char *id){
    char eachUser[1024];
    FILE *pFile=fopen("accounts.txt","r");
    while(fgets(eachUser,sizeof(eachUser),pFile)){
        char tempId[5];
        sscanf(eachUser,"%[^,]",tempId);
        if(strcmp(tempId,id)==0){
            fclose(pFile);
            return 0;
        }
    }
    fclose(pFile);
    return 1;
}
char* generateCode(){
    int max=9999;
    int min=1001;
    static char randomIdInText[5];
    srand(time(NULL));
    int randomId=rand() % (max - min + 1) + min;
    sprintf(randomIdInText,"%d",randomId);
    if(checkUniqency(randomIdInText)){
        return randomIdInText;
    }else{
        generateCode();
    }
}

int flagChange(){
    int c;
    while((c=getchar()), c != '\n' && c != EOF);
    system("cls");
    printf("Blocking and Unblocking of Accounts\n\n");
    char code[5];
    char name[50], country[20],flag[10],pass[10],Code[5],choice;
    int lowpd,lodpd,lot,balance;

    int happen=0;

    printf("Code: ");
    fgets(code,sizeof(code),stdin);
    code[strcspn(code,"\n")]='\0';

    FILE *pFile=fopen("accounts.txt","r");
    FILE *pTempFile=fopen("temp.txt","w");
    char temp[5],ch[1024];
    while(fgets(ch,sizeof(ch),pFile)){
        sscanf(ch,"%4[^,],%49[^,],%9[^,],%19[^,],%d,%d,%d,%d,%9[^,\n]",Code,name,pass,country,&lowpd,&lodpd,&lot,&balance,flag);
        flag[strcspn(flag,"\n")]='\0';
        Code[strcspn(Code, "\n")] = '\0';
        if(strcmp(Code,code)==0){
            happen=1;
            if(strcmp(flag,"green")==0){
                printf("\nThe account %s is Active\nDo you want to Block it? (y=yes and n=no): ",Code);
                scanf(" %c",&choice);
                while((c=getchar()), c != '\n' && c != EOF);
                if(tolower(choice)=='y'){
                    strcpy(flag,"red");
                    happen=2;
                } else happen=4;
            }else{
                printf("\nThe account %s is Blocked\nDo you want to Unblock it? (y=yes and n=no): ",Code);
                scanf(" %c",&choice);
                while((c=getchar()), c != '\n' && c != EOF);
                if(tolower(choice)=='y'){
                    strcpy(flag,"green");
                    happen=3;
                }else happen=4;
            }
        }

        fprintf(pTempFile,"%s,%s,%s,%s,%d,%d,%d,%d,%s\n",Code,name,pass,country,lowpd,lodpd,lot,balance,flag);
        
    }
    fclose(pFile);
    fclose(pTempFile);
    remove("accounts.txt");
    rename("temp.txt","accounts.txt");

    if(happen==0){
        printf("\nThe Account not found\n");
        Sleep(5000);
        return 0;
    }
    if(happen>=1){
        if(happen){
            switch (happen)
            {
            case 2:
                printf("We blocked %s\n",Code);
                Sleep(5000);
                break;
            case 3:
                printf("We Un blocked %s\n",Code);
                Sleep(5000);
                break;
            case 4:
                printf("No changed to %s\n",Code);
                Sleep(5000);
                break;
            }
        }
    }
    return 0;
}
int updateAccount(){
    int c;
    while((c=getchar()), c != '\n' && c != EOF);
    system("cls");
    printf("Updating Client's Account\n\n");
    char code[5];
    char name[50], country[20],flag[10],pass[10],Code[5],choice;
    int lowpd,lodpd,lot,balance;

    printf("Code: ");
    fgets(code,sizeof(code),stdin);
    code[strcspn(code,"\n")]='\0';

    FILE *pFile=fopen("accounts.txt","r");
    FILE *pTempFile=fopen("temp.txt","w");
    char temp[5],ch[1024];

    int happen=0,chose,isDoneUpdating=0;

    while(fgets(ch,sizeof(ch),pFile)){
        sscanf(ch,"%4[^,],%49[^,],%9[^,],%19[^,],%d,%d,%d,%d,%9[^,\n]",Code,name,pass,country,&lowpd,&lodpd,&lot,&balance,flag);
        Code[strcspn(Code, "\n")] = '\0';
        if(strcmp(code,Code)==0){
            happen= 1;
            while(1){
                if(isDoneUpdating) break;
                printf("\n\nUpdating information of client %s\n",code);
                printf("What information you want to change of client\n");
                printf("1. Name\n2. Limit of withdrawn per day\n3. Limit of deposit per day\n4. Limit of transfer\n5. Done with updating\nChose: ");
                scanf("%d",&chose);
                switch(chose){
                    case 1:
                        printf("The current name of client %s is %s\n",Code,name);
                        printf("Enter the new name for client %s: ",Code);
                        while((c=getchar()), c != '\n' && c != EOF);
                        fgets(name,sizeof(name),stdin);
                        name[strcspn(name,"\n")]='\0';
                        break;
                    case 2:
                        printf("The current 'Limit of withdrawn per day' of client %s is %d\n",Code,lowpd);
                        printf("Enter the new 'Limit of withdrawn per day' for client %s: ",Code);
                        while((c=getchar()), c != '\n' && c != EOF);
                        if(scanf("%d",&lowpd) != 1){
                            printf("Invalid input");
                            while((c=getchar()), c != '\n' && c != EOF);
                            Sleep(5000);
                            continue;
                        }
                        break;
                    case 3:
                        printf("The current 'Limit of Deposit per day' of client %s is %d\n",Code,lodpd);
                        printf("Enter the new 'Limit of Deposit per day' for client %s: ",Code);
                        while((c=getchar()), c != '\n' && c != EOF);
                        if(scanf("%d",&lodpd) != 1){
                            printf("Invalid input");
                            while((c=getchar()), c != '\n' && c != EOF);
                            Sleep(5000);
                            continue;
                        }
                        break;
                    case 4:
                        printf("The current 'Limit of transfer' of client %s is %d\n",Code,lot);
                        printf("Enter the new 'Limit of transfer' for client %s: ",Code);
                        while((c=getchar()), c != '\n' && c != EOF);
                        if(scanf("%d",&lot) != 1){
                            printf("Invalid input");
                            while((c=getchar()), c != '\n' && c != EOF);
                            Sleep(5000);
                            continue;
                        }
                        break;
                    case 5:
                        isDoneUpdating=1;
                        break;
                    default:
                        printf("Invalid input. Please chose from above");
                        while((c=getchar()), c != '\n' && c != EOF);
                        Sleep(5000);
                        continue;
                        break;
                }
            }
        }
        fprintf(pTempFile,"%s,%s,%s,%s,%d,%d,%d,%d,%s\n",Code,name,pass,country,lowpd,lodpd,lot,balance,flag);
    }

    fclose(pFile);
    fclose(pTempFile);
    remove("accounts.txt");
    rename("temp.txt","accounts.txt");

    if(happen==0){
        printf("\nWe Couldn't found client %s.\n",code);
        Sleep(5000);
    }else{
        printf("\nSuccessfully Updated information of client %s.",code);
        Sleep(5000);
    }
    return 0;
}
void viewAccounts(){
    int c;
    while((c=getchar()), c != '\n' && c != EOF);
    system("cls");
    printf("Viewing Account\n\n");
    char code[5];
    char name[50], country[20],flag[10],pass[10],Code[5],choice,getch;
    int lowpd,lodpd,lot,balance,onceAsked=0,found=0;

    FILE *pFile=fopen("accounts.txt","r");
    char temp[5],ch[1024];

    do{
        if(onceAsked) printf("Invalid Input: ");
        if(!onceAsked) printf("\nDo you want to view all accounts or one specific account? Chose Y for one specific or CHose N for All: ");
        onceAsked=1;
        scanf(" %c",&choice);
        while((c=getchar()), c != '\n' && c != EOF);
    }while(toupper(choice)!='Y' && toupper(choice)!='N') ;

    if(toupper(choice)=='Y'){
        printf("Code: ");
        fgets(code,sizeof(code),stdin);
        code[strcspn(code,"\n")]='\0';
        while(fgets(ch,sizeof(ch),pFile)){
            sscanf(ch,"%4[^,],%49[^,],%9[^,],%19[^,],%d,%d,%d,%d,%9[^,\n]",Code,name,pass,country,&lowpd,&lodpd,&lot,&balance,flag);
            Code[strcspn(Code, "\n")] = '\0';
            if(strcmp(code,Code)==0){
                printf("Code\tName\tLimit Of withdrawn\tLimit of deposit\tLimit of transfer\tBalance\t\tFlag\n");
                found=1;
                printf("%s\t%s\t\t%d\t\t%d\t\t\t%d\t\t\t%d\t\t%s\n",Code,name,lowpd,lodpd,lot,balance,flag);
                while((c=getchar()), c != '\n' && c != EOF);
                getch=getchar();
            }
        }
        if(!found){
            printf("\nThe client %s did not found \n",code);
        }
    
    }else{
        printf("Code\tName\tLimit Of withdrawn\tLimit of deposit\tLimit of transfer\tBalance\t\tFlag\n");
        while(fgets(ch,sizeof(ch),pFile)){
        sscanf(ch,"%4[^,],%49[^,],%9[^,],%19[^,],%d,%d,%d,%d,%9[^,\n]",Code,name,pass,country,&lowpd,&lodpd,&lot,&balance,flag);
        Code[strcspn(Code, "\n")] = '\0';
        printf("%s\t%s\t\t%d\t\t%d\t\t\t%d\t\t\t%d\t\t%s\n",Code,name,lowpd,lodpd,lot,balance,flag);
        }
        getch=getchar();
    }
}