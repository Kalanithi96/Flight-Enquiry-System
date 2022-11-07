#include"admin.h"       //Header File that contains Functions Performed by an ADMIN USER
#include"customer.h"    //Header File that contains Functions Performed by a CUSTOMER
int main()
{
    int log;    //Variable to choose the USER TYPE
    do  //LOOP till User wants to Exit
    {
        do  //MENU TO CHOOSE USER TYPE
        {
            printf("\nFlight Enquiry System:\n");
            printf("\nEnter the type of User:");
            printf("\n\t1. Admin");
            printf("\n\t2. Customer");
            printf("\n\t3. Exit\n");
            printf("\nEnter your choice: ");
            scanf("%d",&log);
        }while(log<1||log>3);   //Validation of CHOICE

        if(log!=3)  //Since choice 3 is for EXIT
        {                                   //Array of pointers to functions of Return type int and no Arguments
            int (*user[])()={admin_login,   //admin_login - ADMIN LOGIN function
                             cust_login};   //cust_login  - CUSTOMER LOGIN function
            adm(user[log-1]);             //Callback Function depending on Choice
        }
        else        //Since choice 3 is for EXIT
            exit(0);
    }while(1);

    return 0;   //Program has ended successfully
}   //End of main() function
