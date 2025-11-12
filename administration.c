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
    printf("1. Create a Client Account\n2. Block and Unblock Accounts\n3. Update an client Account\n4. View Accounts\nChose: ");
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
        break;
    case 3:
        break;
    case 4:
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