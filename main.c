    #include "header.h"

    int main() {
        int chose,c;
        while (true)
        {
            printf("** Welcome to ROHYAS BANK**\n");
            printf("Please chose from the below:\n1. Administration\n2. Client\nChose: ");
            if(scanf("%d",&chose) != 1){
                printf("\nInvalid Input\n");
                while((c=getchar()), c != '\n' && c != EOF);
                continue;
            }
            switch (chose)
            {
            case 1:
                adminLogin();
                break;
            case 2:
                
                break;

            default:
                printf("\nInvalid Input\n");
                break;
            }
        }

        return 0;
    }