#include "common.h"
int cust(char*);        //Function to Access Menu Driven Customer Operations
int search_flight();    //Function to Search a Flight by Source or Destination or Departure Day
int cust_login()        //Function for a Customer to Login
{
    int ch;             //Variable to input Choice
    do                  //MENU
    {
        printf("\n\t1. Existing User");
        printf("\n\t2. New User");
        printf("\n\nEnter your choice: "); //Prompt to input ch
        scanf("%d",&ch);                   //Input of ch
    }while(ch<1||ch>2);   //Validating ch: 1 or 2

    if(ch==1)       //If Existing User
    {
        FILE *f;
        LOGIN input,temp;
        int i,tries=3;
        for (i=0;i<tries;i++)       //Loop for 3 tries
        {
            f=fopen("CUST.dat","rb");   //Opening CUST.dat that contains Customer IDs and Passwords
            if(f==NULL)                 //If error in opening File
            {
                printf("Error Opening FILE!!!");
                return 0;
            }
            printf("\nEnter Login ID: ");   //Prompt to input Login ID
            fflush(stdin);                  //Flushing STDIN
            scanf("%s",input.id);           //Input of ID
            input_pw(input.pw);             //Input of Password
            while(fread(&temp,sizeof(input),1,f))   //Reading ONE Customer ID at a time
            {
                if(strcmp(temp.id,input.id)==0)     //IF Input ID and read ID match
                    if(strcmp(temp.pw,input.pw)==0)     //If Input password and read password match
                    {
                         if(cust(temp.id))                  //Login Successful
                            return 0;                           //After Login ends, Return to Login Page
                    }
                    else                                //If Input password and read password do NOT match
                    {
                        printf("ID and Password do not match!!!\n");    //Display Not matched
                        break;
                    }
            }
            printf("Trials left: %d",tries-i-1);        //Login Failure: No ID Found or ID Found & Password not matches
            if(i==2)        //If 3 trials over
                exit(0);    //Exit Program
            fclose(f);      //Closing CUST.dat
        }
    }

    else        //If New User
    {
        FILE *f,*f2;
        LOGIN new_id,temp;
        f=fopen("CUST.dat","ab");                   //Opening CUST.dat in Append Mode
        if(f==NULL)                                 //If Error in opening File
        {
            printf("Error Opening FILE!!!");
            return 0;                               //Return to Login Page
        }
        do                                          //Loop to Validate ID to be Unique
        {
            printf("Enter your ID: ");              //Prompt to input ID
            fflush(stdin);                          //Flushing STDIN
            scanf("%s",new_id.id);                  //Input of ID
            f2=fopen("CUST.dat","r");               //Opening CUST.dat in READ-ONLY mode
            if(f2==NULL)                            //If Error in opening file
            {
                printf("Error Opening FILE!!!");    //Error Message
                return 0;                           //Return to Login Page
            }
            while(fread(&temp,sizeof(temp),1,f2))   //Reading ONE ID at a time
                if(strcmp(temp.id,new_id.id)==0)    //If ID matches
                    break;                          //Break the Loop
        }while(!feof(f2));      //If End of File, Input ID is Unique, Condition False, Loop Ends
                                //So, Wrong Input if NOT End of File, Hence Loop continues
        fclose(f2);                                 //Closing CUST.dat opened in READ-ONLY Mode
        input_pw(new_id.pw);                        //Input of Password
        fwrite(&new_id,sizeof(new_id),1,f);         //Append the Validated New ID
        fclose(f);                                  //Closing CUST.dat opened in append mode
        printf("\nNEW Customer ID Created!!\n");    //Message to user
        cust(new_id.id);                            //Logging in using new ID
    }
}
int cust(char name[])                               //Function to Access Menu Driven Customer Operations
{
    int ch;                                         //Choice ch
    if(islower(name[0]))                            //If 1st letter of name is Lower case alphabet
        name[0]=toupper(name[0]);                       //Convert to Upper case
    printf("\n\nWelcome %s\n",name);                //Welcome Message
    do                                              //Loop to Make operations
    {
        do                                          //Loop to validate Input of Choice
        {
            printf("\nMENU");
            printf("\n\t1. Display all Airports");
            printf("\n\t2. Search Flight");
            printf("\n\t3. Display all Flights");
            printf("\n\t4. Return to Login Page");
            printf("\nEnter your choice: ");        //Prompt to input ch
            scanf("%d",&ch);                        //Input of ch
            printf("\n");
        }while(ch<1||ch>4);      //Validating ch: 1-4

        if(ch!=4)               //If Choice is not Return to Login page
        {                                       //Array of pointers to functions of Return type int and no Arguments
            int (*user[])()={display_airports,  //display_airports - To Display all Airports
                             search_flight,     //search_flight - To Search Flights
                             dis_all_flights};  //dis_all_flights - To Display details of all Flights
            adm(user[ch-1]);        //Callback function depending on choice
        }
        else                    //If Choice is Return to Login page
            return 1;               //Return to Login page

    }while(1);                  //Loop continues till User chooses not to
}
int search_flight()             //Function to Search a Flight by Source or Destination or Departure Day
{
    char from[4],to[4];
    int i,j,day,ch;
    FLIGHT *temp;               //Pointer to a FLIGHT type variable
    temp=(FLIGHT*)malloc(sizeof(FLIGHT));   //Dynamic Allocation of (sizeof(FLIGHT)) bytes
    FILE *f;                    //FILE pointer f
    f=fopen("FLIGHT.dat","rb"); //Opening FLIGHT.dat in READ-ONLY mode
    if(f==NULL)                 //If Error in opening th file
    {
        printf("Error in Opening File!!!"); //Error message
        return 0;                           //Return to Customer page
    }
    j=1;                        //Number of Flights found
    do                          //Loop to validate ch
    {
        printf("SEARCH BY:");
        printf("\n\t1.FROM");   //Search by FROM
        printf("\n\t2.TO");     //Search by TO
        printf("\n\t3.DAY");    //Search by Day of Departure
        printf("\nChoose: ");   //Prompt to input ch
        scanf("%d",&ch);        //Input of ch
    }while(ch<1||ch>3);         //True for wrong inputs, Right inputs: 1-3
    switch(ch)
    {                                       //For Choice 1
        case 1: display_airports();                         //Display all Airports for user to Choose from
                do                                          //Loop to validate Input of FROM
                {
                    printf("Enter the From location: ");    //Prompt to input FROM
                    fflush(stdin);                          //Flushing STDIN
                    scanf("%3s",from);                      //Input of FROM
                    for(i=0;i<3;i++)
                        if(!isalpha(from[i]))               //If a character is not an alphabet
                            break;                              //Break the loop, i NOT equal to 3
                }while(strlen(from)!=3||i!=3);      //Wrong input: length of FROM is NOT 3, a Character of FROM not an alphabet
                                                    //True for Wrong input, Loop continues till Correct Input
                for(i=0;i<3;i++)                    //For all Characters of FROM
                    if(islower(from[i]))                //If the character is Lower case
                        from[i]=toupper(from[i]);           //Conversion to Upper case

                while(fread(temp,sizeof(FLIGHT),1,f))   //Read ONE Flight at a time
                {
                    if(strcmp(temp->from,from)==0)      //If Input FROM matches the read Flight's FROM
                    {
                        printf("\nFlight %d:\n\n",j++);
                        display_flight(temp);           //Display the Flight details
                        printf("-------------------------------------------------------\n");
                    }
                }               //Loop continues till All records are read
                break;
                                            //For Choice 2
        case 2: display_airports();                         //Display all Airports for user to Choose from
                do                                          //Loop to validate Input of TO
                {
                    printf("Enter the Destination: ");      //Prompt to input TO
                    fflush(stdin);                          //Flushing STDIN
                    scanf("%3s",to);                        //Input of TO
                    for(i=0;i<3;i++)
                        if(!isalpha(to[i]))                 //If a character is not an alphabet
                            break;                              //Break the loop, i NOT equal to 3
                }while(strlen(to)!=3||i!=3);        //Wrong input: length of TO is NOT 3, a Character of TO not an alphabet
                                                    //True for Wrong input, Loop continues till Correct Input
                for(i=0;i<3;i++)                    //For all Characters of TO
                    if(islower(to[i]))                  //If the character is Lower case
                        to[i]=toupper(to[i]);               //Conversion to Upper case

                while(fread(temp,sizeof(FLIGHT),1,f))   //Read ONE Flight at a time
                {
                    if(strcmp(temp->to,to)==0)          //If Input TO matches the read Flight's TO
                    {
                        printf("\nFlight %d:\n\n",j++);
                        display_flight(temp);           //Display the Flight details
                        printf("-------------------------------------------------------\n");
                    }
                }               //Loop continues till All records are read
                break;
                                            //For Choice 3
        case 3: printf("Enter Index of the Departure day in a week [1-7]: ");   //Prompt to input DAY of Departure
                fflush(stdin);              //Flushing STDIN
                scanf("%d",&day);           //Input of DAY of Departure

                while(fread(temp,sizeof(FLIGHT),1,f))   //Read ONE Flight at a time
                {
                    if(day==temp->departure.day)        //If Input DAY matches the read Flight's DAY
                    {
                        printf("\nFlight %d:\n\n",j++);
                        display_flight(temp);           //Display the Flight details
                        printf("-------------------------------------------------------\n");
                    }
                }               //Loop continues till All records are read
    }
    printf("\n( %d ) FLIGHT(S) FOUND AND DISPLAYED!!\n",j-1);   //Display of Number of Flights Found matching
    fclose(f);          //Closing FLIGHT.dat
    free(temp);         //Deallocating memory allocated to temp
}
