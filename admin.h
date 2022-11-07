#include "common.h"
int admin(char*);           //Function to Access Menu Driven ADMIN Operations
int add_admin();            //Function to Add an ADMIN user ID
int add_flight();           //Function to Add a FLIGHT record
int rem_flight();           //Function to Remove a FLIGHT record
int add_ap();               //Function to Add a AIRPORT record
int rem_ap();               //Function to Remove a AIRPORT record
int edit_flight();          //Function to Edit details of a FLIGHT record
int sort();                 //Function to Sort the records in FLIGHT.dat
int sort_airport();         //Function to Sort the records in AIRPORT.dat
void check_airport(char*);  //Function to Check and Append an Airport ID
void read_time(TIME*);      //Function to Input a variable of type TIME

int admin_login()           //Function for an ADMIN to login
{
    FILE *f,*f1;
    LOGIN input,temp;
    int i=3,tries=3;
    f1=fopen("ADMIN.dat","ab+");
    fread(&input,sizeof(input),1,f1);
    if(feof(f1))
        add_admin();
    else
    {
        for(i=0;i<tries;i++)            //Maximum Number of tries is 3
        {
            f=fopen("ADMIN.dat","rb");          //Opening ADMIN.dat in READ-ONLY mode
            if(f==NULL)                         //If error in opening the file
            {
                printf("Error Opening FILE!!!"); //Error Message
                return 0;                        //Return to Login Page
            }
            printf("\nEnter Login ID: ");       //Prompt to Input Login ID
            fflush(stdin);                      //Flushing STDIN
            scanf("%s",input.id);               //Input of ID
            input_pw(input.pw);                     //Input of Password
            while(fread(&temp,sizeof(input),1,f))   //Reading ONE ID at a time
            {
                if(strcmp(temp.id,input.id)==0)     //IF Input ID and read ID match
                    if(strcmp(temp.pw,input.pw)==0)     //If Input password and read password match
                    {
                        if(admin(temp.id))                  //Admin Login Successful
                            return 0;                           //After Login ends, Return to Login Page
                    }
                    else                                //If Input password and read password do NOT match
                    {
                        printf("ID and Password do not match!!!");
                        break;                          //Then End the loop
                    }
            }
            printf("Trials left: %d",tries-i-1);    //Login Failure: No ID Found or ID Found & Password not matches
            if(i==2)        //If 3 trials over
                exit(0);    //Exit Program
            fclose(f);      //Closing ADMIN.dat
        }
    }
}
int admin(char name[])                              //Function to Access Menu Driven Admin Operations
{
    int ch;                                         //Choice ch
    if(islower(name[0]))                            //If 1st letter of name is Lower case alphabet
        name[0]=toupper(name[0]);                       //Convert to Upper case
    printf("\n\nWelcome %s\n",name);                //Welcome Message
    do                                              //Loop to Make operations
    {
        do                                          //Loop to validate Input of Choice
        {
            printf("\nMENU");                       //MENU
            printf("\n\t 1. Add Admin User");
            printf("\n\t 2. Add Flight");
            printf("\n\t 3. Remove Flight");
            printf("\n\t 4. Edit Flight");
            printf("\n\t 5. Sort Flights");
            printf("\n\t 6. Display All Flights");
            printf("\n\t 7. Add Airport");
            printf("\n\t 8. Remove Airport");
            printf("\n\t 9. Display All Airports");
            printf("\n\t10. Return to Login Page");
            printf("\nEnter your choice: ");        //Prompt to input ch
            scanf("%d",&ch);                        //Input of ch
            printf("\n");
        }while(ch<1||ch>10);    //Wrong input: Condition is True: Loop continues
                                //Right Input: ch is [1-10] : Loop ends
        if(ch!=10)           //If Choice is NOT return to Login Page
        {                                       //Array of pointers to functions of Return type int and no Arguments
            int (*user[])()={add_admin,         //Adding an ADMIN user ID
                             add_flight,        //Adding a FLIGHT record
                             rem_flight,        //Removing a FLIGHT record
                             edit_flight,       //Editing a FLIGHT record
                             sort,              //Sorting the FLIGHT.dat file
                             dis_all_flights,   //Displaying all the FLIGHT records
                             add_ap,            //Adding an AIRPORT record
                             rem_ap,            //Removing an AIRPORT record
                             display_airports}; //Displaying all the AIRPORT records
            adm(user[ch-1]);        //Callback function depending on choice
        }
        else                    //If Choice is Return to Login page
            return 1;               //Return to Login page

    }while(1);      //Loop continues till the USER chooses not to
}
int add_admin()                             //Function to add a NEW ADMIN user ID
{
    FILE *f,*f2;
    LOGIN new_id,temp;
    f=fopen("ADMIN.dat","ab");              //Opening ADMIN.dat in APPEND mode
    if(f==NULL)                             //If error in opening the file
    {
        printf("Error in Opening File!!!"); //Error message
        return 0;                           //Return to Login Page
    }
    do                                      //Loop to validate the input of ID
    {
        printf("Enter the ID: ");           //Prompt to input ID
        fflush(stdin);                      //Flushing STDIN
        gets(new_id.id);                    //Input of ID
        f2=fopen("ADMIN.dat","rb");          //Opening ADMIN.dat in READ-ONLY mode
        if(f2==NULL)                        //If error in opening the file
        {
            printf("Error in Opening File!!!"); //Error message
            return 0;                           //Return to Login Page
        }
        while(fread(&temp,sizeof(temp),1,f2))   //Reading ONE Flight at a time
                if(strcmp(temp.id,new_id.id)==0)    //If read ID and Input ID match
                    break;                              //The input ID is not unique and loop ends
    }while(!feof(f2));          //If input ID is not unique, f2 does not reach end of file
                                //Condition is TRUE, the Loop continues
                                //If ID id unique, the Loop breaks
    fclose(f2);                 //Closing ADMIN.dat opened in READ-ONLY mode
    input_pw(new_id.pw);        //Input of Password
    fwrite(&new_id,sizeof(new_id),1,f);     //Appending the new ADMIN ID record
    fclose(f);                  //Closing ADMIN.dat opened in APPEND mode
}
int add_flight()                            //Function to add a NEW Flight record
{
    FILE *f,*f2;
    char chk;
    int i;
    FLIGHT new_flight,temp;
    f=fopen("FLIGHT.dat","ab+");                //Opening FLIGHT.dat in APPEND and READ mode
    if(f==NULL)                                 //If error in opening the file
    {
        printf("Error in Opening File!!!");     //Error message
        return 0;                               //Return to Admin Page
    }
    do                                          //Loop to validate input of FLIGHT ID
    {
        printf("Enter the ID: ");               //Prompt to input Flight ID
        fflush(stdin);                          //Flushing STDIN
        gets(new_flight.id);                    //Input of ID
        f2=fopen("FLIGHT.dat","rb");            //Opening FLIGHT.dat in READ-ONLY mode
        if(f2==NULL)                            //If error in opening the file
        {
            printf("Error in Opening File!!!"); //Error Message
            return 0;                           //Return to Admin page
        }
        while(fread(&temp,sizeof(temp),1,f2))   //Reading ONE Flight at a time
            if(strcmp(temp.id,new_flight.id)==0)    //If Input ID and Read ID Match: Input ID is not UNIQUE
                break;                                  //f2 NOT at End of File: breaking the loop
    }while(!feof(f2));                          //Continue Loop till Unique ID is Input
    fclose(f2);                                 //Close FLIGHT.dat opened in READ-ONLY mode
    do                                          //Loop to Validate the input of Flight Name
    {
        printf("Enter the Name of Flight: ");   //Prompt to input FLIGHT Name
        fflush(stdin);                          //Flushing STDIN
        gets(new_flight.name);                  //Input of Flight Name
        if(!isalpha(new_flight.name[0]))        //If First letter of Name is NOT an Alphabet: Wrong Input
            printf("INVALID NAME: Name must start with an Alphabet\n"); //Error Message
    }while(!isalpha(new_flight.name[0]));       //Wrong Input: Condition True: Loop continues until Right Input
    if(islower(new_flight.name[0]))             //If First letter of Name is Lower case
        new_flight.name[0]=toupper(new_flight.name[0]); //Convert it to Upper case
    for(i=1;i<strlen(new_flight.name)-1;i++)                        //For Every character from 2nd Letter of Name
        if(new_flight.name[i]==' '&&islower(new_flight.name[i+1]))      //If First Letter of a word id Lower case
            new_flight.name[i+1]=toupper(new_flight.name[i+1]);             //CONVERT it to Upper Case
    display_airports();                         //Displaying all the Airports
    do                                          //Loop to validate the input of FROM
    {
        printf("Enter the From location: ");    //Prompt to input FROM
        fflush(stdin);                          //Flushing STDIN
        scanf("%3s",new_flight.from);           //Input of FROM
        for(i=0;i<3;i++)                        //For every letter in FROM
            if(!isalpha(new_flight.from[i]))        //If the letter is not an Alphabet
                break;                                  //i NOT equal to 3: Ending the loop
    }while(strlen(new_flight.from)!=3||i!=3);   //Wrong Input: A letter of FROM is not an Alphabet
                                                //             Length of FROM is not THREE : Loop continues till Right Input
    for(i=0;i<3;i++)                            //For Every letter of FROM
        if(islower(new_flight.from[i]))             //If it is Lower case
            new_flight.from[i]=toupper(new_flight.from[i]); //Convert it to Upper case
    check_airport(new_flight.from);             //Check if FROM already exists. Else, Add it to AIRPORT.dat
    do                                          //Loop to validate the input of TO
    {
        printf("Enter the Destination: ");      //Prompt to input TO
        fflush(stdin);                          //Flushing STDIN
        scanf("%3s",new_flight.to);             //Input of TO
        for(i=0;i<3;i++)                        //For every letter in TO
            if(!isalpha(new_flight.to[i]))          //If the letter is not an Alphabet
                break;                                  //i NOT equal to 3: Ending the loop
    }while(strlen(new_flight.to)!=3||i!=3);     //Wrong Input: A letter of TO is not an Alphabet
                                                //             Length of TO is not THREE : Loop continues till Right Input
    for(i=0;i<3;i++)                            //For Every letter of TO
        if(islower(new_flight.to[i]))               //If it is Lower case
            new_flight.to[i]=toupper(new_flight.to[i]);     //Convert it to Upper case
    check_airport(new_flight.to);               //Check if TO already exists. Else, Add it to AIRPORT.dat
    printf("Departure Time:\n");                //Prompt to input Departure Time
    read_time(&(new_flight.departure));         //Reading Departure Time
    printf("Arrival Time:\n");                  //Prompt to input Arrival Time
    read_time(&(new_flight.arrival));           //Reading Arrival Time
    printf("Flight details being added:\n");
    printf("-------------------------------------------------------\n");
    display_flight(&new_flight);                //Displaying the Details of the New Flight to be Added
    printf("CONFIRM?[Y/N] ");                   //Prompt to Input Confirmation
    fflush(stdin);                              //Flushing STDIN
    scanf("%c",&chk);                           //Input of Confirmation
    if(chk=='y'||chk=='Y')                      //If Yes
    {
        fwrite(&new_flight,sizeof(new_flight),1,f); //Append the Details of New Flight to FLIGHT.dat
        printf("1 Flight Record ADDED!!\n");        //Message to user
    }
    fclose(f);      //Closing FLIGHT.dat opened in "ab+" mode
}
int rem_flight()                            //Function to Remove a Flight record
{
    FILE *f,*f1;
    char del[20],chk='\0';                  //chk is assigned a value that can only be changed in case a match is found
    FLIGHT temp;
    f=fopen("FLIGHT.dat","rb");             //Opening FLIGHT.dat in READ-ONLY mode
    if(f==NULL)                             //If error in opening the file
    {
        printf("Error in Opening File!!!"); //Error Message
        return 0;                           //Return to Admin Page
    }
    f1=fopen("TEMP.dat","wb");              //Opening TEMP.dat in WRITE mode as a Temporary File
    printf("Enter the ID: ");               //Prompt to Input ID of Flight record to be removed
    fflush(stdin);                          //Flushing STDIN
    scanf("%s",del);                        //Input of the ID
    while(fread(&temp,sizeof(temp),1,f))    //Reading ONE Flight record at a time
    {
        if(strcmp(temp.id,del)!=0)          //If Read ID and the Input ID do NOT match
            fwrite(&temp,sizeof(temp),1,f1);    //Write the Flight Record into TEMP.dat
        else                                //If Read ID and the Input ID match
        {
            printf("Flight details being Removed:\n");
            printf("-------------------------------------------------------\n");
            display_flight(&temp);          //Displaying the details of the record to be removed
            printf("CONFIRM?[Y/N] ");       //Prompt to input Confirmation
            fflush(stdin);                  //Flushing STDIN
            scanf("%c",&chk);               //Input of Confirmation
            if(chk=='n'||chk=='N')          //If NO
                fwrite(&temp,sizeof(temp),1,f1);    //Write the Flight Record into TEMP.dat
            else                            //Else, The record is skipped
                printf("1 Flight Record REMOVED!!\n");  //Message to User
        }
    }                       //Loop ends after all the records of FLIGHT.dat are read
    if(chk=='\0')           //If chk is unchanged
        printf("RECORD NOT FOUND!!!");  //No record found with input ID
    fclose(f);              //Closing FLIGHT.dat
    fclose(f1);             //Closing TEMP.dat
    remove("FLIGHT.dat");   //Deleting the FLIGHT.dat
    rename("TEMP.dat","FLIGHT.dat");    //Renaming TEMP.dat to FLIGHT.dat
}
int edit_flight()                           //Function to Edit a Flight record
{
    FILE *f,*f2;
    char edit[20],chk;
    int i;
    FLIGHT temp,tem;
    int ch,rec=0;                               //rec - To keep track of the records
    f=fopen("FLIGHT.dat","rb+");                //Opening FLIGHT.dat in READ-ONLY mode
    if(f==NULL)                                 //If Error in opening the file
    {
        printf("Error in Opening File!!!");     //Error Message
        return 0;                               //Return to Admin page
    }
    rewind(f);                                  //Move f to beginning of file
    printf("Enter the ID: ");                   //Prompt to input ID of Flight record to be edited
    fflush(stdin);                              //Flushing STDIN
    scanf("%s",edit);                           //Input of Flight ID
    while(fread(&temp,sizeof(temp),1,f))        //Reading ONE Flight record at a time
    {
        if(strcmp(temp.id,edit)==0)             //If Input ID and Read ID match
        {
            do                                  //Loop to Edit the details
            {
                printf("OLD FLIGHT DETAILS:\n");
                display_flight(&temp);          //Displaying Old Details of Flight to be Edited
                printf("\n");
                printf("Detail to be edited:"); //MENU
                printf("\n\t1. Flight ID");
                printf("\n\t2. Flight Name");
                printf("\n\t3. From");
                printf("\n\t4. To");
                printf("\n\t5. Departure");
                printf("\n\t6. Arrival");
                do                              //Loop to validate the input of Choice ch
                {
                    printf("\nChoose: ");       //Prompt to input choice
                    scanf("%d",&ch);            //Input of Choice
                }while(ch<1||ch>6);             //Wrong Input: ch NOT in [1-6]: Loop continues till Right input
                switch(ch)
                {                   //Choice 1: ID
                    case 1: do      //Loop to validate the Input of ID
                            {
                                printf("Enter the new ID: ");   //Prompt to Input ID
                                fflush(stdin);                  //Flushing STDIN
                                scanf("%s",temp.id);            //Input of ID
                                f2=fopen("FLIGHT.dat","rb");    //Opening FLIGHT.dat in READ-ONLY mode
                                if(f2==NULL)                    //Error in Opening the file
                                {
                                    printf("Error in Opening File!!!"); //Error Message
                                    return 0;                   //Return to Admin Page
                                }
                                while(fread(&tem,sizeof(tem),1,f2)) //Reading ONE Flight record at a time
                                    if(strcmp(temp.id,tem.id)==0)   //If Input ID and Read ID match: Input ID is NOT UNIQUE
                                        break;                          //f2 NOT at End of File: Loop ending
                            }while(!feof(f2));      //Wrong Input: ID not UNIQUE: Loop continues till Unique ID is input
                            fclose(f2);             //Closing FLIGHT.dat opened by f2
                            break;
                                    //Choice 2: NAME
                    case 2: do      //Loop to validate the Input of Name
                            {
                                printf("Enter the new name: ");         //Prompt to Input Name
                                fflush(stdin);                          //Flushing STDIN
                                scanf("%s",temp.name);                  //Input of Name
                                if(!isalpha(temp.name[0]))              //If First letter is not an Alphabet: Wrong input
                                    printf("INVALID NAME: Name must start with an Alphabet\n"); //Error Message
                            }while(!isalpha(temp.name[0]));             //Wrong Input: Condition True: Loop Continues till Right Input
                            if(islower(temp.name[0]))                   //If First Letter of Name is Lower Case
                                temp.name[0]=toupper(temp.name[0]);         //Convert it to Upper Case
                            for(i=1;i<strlen(temp.name)-1;i++)          //For Every Character from 2nd Letter of Name
                                if(temp.name[i]==' '&&islower(temp.name[i+1]))  //If First Letter of a Word is Lower case
                                    temp.name[i+1]=toupper(temp.name[i+1]);         //Convert it to upper case
                            break;
                                    //Choice 3: FROM
                    case 3: do      //Loop to validate the Input of From
                            {
                                printf("Enter the From location: ");    //Prompt to Input From
                                fflush(stdin);                          //Flushing STDIN
                                scanf("%3s",temp.from);                 //Input of From
                                for(i=0;i<3;i++)                        //For every letter of From
                                    if(!isalpha(temp.from[i]))              //If it is NOT an alphabet
                                        break;                                  //i NOT equal to 3: Wrong Input
                            }while(strlen(temp.from)!=3||i!=3);         //Wrong Input: Length of FROM is NOT THREE
                                                                        //             A letter of FROM is not an Alphabet
                                                                        //Loop continues till Right INPUT
                            for(i=0;i<3;i++)                            //For every letter of FROM
                                if(islower(temp.from[i]))                   //If it is Lower case
                                    temp.from[i]=toupper(temp.from[i]);         //Convert it to Upper Case
                            check_airport(temp.from);                   //Check if FROM already exists. Else, Add it to AIRPORT.dat
                            break;
                                    //Choice 4: TO
                    case 4: do      //Loop to validate the Input of To
                            {
                                printf("Enter the new Destination: ");  //Prompt to Input To
                                fflush(stdin);                          //Flushing STDIN
                                scanf("%3s",temp.to);                   //Input of To
                                for(i=0;i<3;i++)                        //For every letter of To
                                    if(!isalpha(temp.to[i]))            //If it is NOT an alphabet
                                        break;                                  //i NOT equal to 3: Wrong Input
                            }while(strlen(temp.to)!=3||i!=3);           //Wrong Input: Length of TO is NOT THREE
                                                                        //             A letter of TO is not an Alphabet
                                                                        //Loop continues till Right INPUT
                            for(i=0;i<3;i++)                            //For every letter of TO
                            if(islower(temp.to[i]))                         //If it is Lower case
                                temp.to[i]=toupper(temp.to[i]);                 //Convert it to Upper Case
                            check_airport(temp.to);                     //Check if TO already exists. Else, Add it to AIRPORT.dat
                            break;
                                    //Choice 5: DEPARTURE TIME
                    case 5: printf("Enter the new Departure time: ");   //Prompt to input Departure Time
                            read_time(&(temp.departure));               //Input of Departure Time
                            break;
                                    //Choice 6: ARRIVAL TIME
                    case 6: printf("Enter the new Arrival time: ");     //Prompt to Input Arrival Time
                            read_time(&(temp.arrival));                 //Input of Arrival Time
                            break;
                }
                printf("Do you wish to Edit any other detail? [Y/N] "); //Prompt to Continue or not
                fflush(stdin);                                          //Flushing STDIN
                scanf("%c",&chk);                                       //Input of chk
            }while(chk=='y'||chk=='Y');         //Loop Continues till User is done editing and does not enter Y
            printf("\nNew Flight details:\n");
            printf("-------------------------------------------------------\n");
            display_flight(&temp);              //Displaying the New Details of the Flight
            printf("CONFIRM?[Y/N] ");           //Prompt to input Confirmation
            fflush(stdin);                      //Flushing STDIN
            scanf("%c",&chk);                   //Input of Confirmation
            printf("\n");
            if(chk=='y'||chk=='Y')              //If YES
            {
                fseek(f,rec*sizeof(temp),0);            //Move f to the "rec" th record
                fwrite(&temp,sizeof(temp),1,f);         //Overwrite the New Details onto the Old ones
                printf("1 Flight Record EDITED!!\n");   //Message to user
            }
            break;                              //Ending Loop once Editing is done
        }
        rec++;                                  //Rec incremented if Flight Record not found in current Iteration
    }
    if(feof(f))                                 //If f reached End of File, no matching Record found
        printf("RECORD NOT FOUND!!\n");             //Message to user
    fclose(f);                                  //Closing FLIGHT.dat
}
int add_ap()                                //Function to Add an Airport record
{
    FILE *f,*f2;
    char chk;
    int i;
    AIRPORT new_ap,temp;
    f=fopen("AIRPORT.dat","ab+");               //Opening AIRPORT.dat in APPEND and READ mode
    if(f==NULL)                                 //If error in opening the file
    {
        printf("Error in Opening File!!!");     //Error message
        return 0;                               //Return to Admin Page
    }
    do                                          //Loop to validate the Uniqueness of input AIRPORT ID
    {
        do                                          //Loop to validate the input of AIRPORT ID
        {
            printf("Enter the AIRPORT ID: ");       //Prompt to input ID
            fflush(stdin);                          //Flushing STDIN
            scanf("%3s",new_ap.id);                 //Input of ID
            for(i=0;i<3;i++)                        //For every letter in ID
                if(!isalpha(new_ap.id[i]))          //If the letter is not an Alphabet
                    break;                                  //i NOT equal to 3: Ending the loop
        }while(strlen(new_ap.id)!=3||i!=3);         //Wrong Input: A letter of ID is not an Alphabet
                                                    //             Length of ID is not THREE : Loop continues till Right Input
        for(i=0;i<3;i++)                            //For Every letter of ID, i is set to 3 for valid ID
            if(islower(new_ap.id[i]))                   //If it is Lower case
                new_ap.id[i]=toupper(new_ap.id[i]);         //Convert it to Upper case
        f2=fopen("AIRPORT.dat","rb");               //Opening AIRPORT.dat in READ-ONLY mode
        while(fread(&temp,sizeof(temp),1,f2))       //Reading ONE Airport Record at a time
            if(strcmp(temp.id,new_ap.id)==0)            //If Read ID and Input ID match
            {
                i=0;                                        //The flag "i" is set to 0
                printf("The ID Already exists!!\n");        //Message to User
                break;                                      //Ending loop
            }
        fclose(f2);                                 //Closing FLIGHT.dat opened in READ-ONLY mode
    }while(i==0);               //Wrong Input: Input ID is not UNIQUE

    do          //Loop to validate input of name
    {
        printf("\nEnter the Name of Airport: ");            //Prompt to input Name
        fflush(stdin);                                      //Flushing STDIN
        gets(new_ap.name);                                  //Input of Name
        for(i=0;i<strlen(new_ap.name);i++)                  //For Every character of Name
            if(!isalpha(new_ap.name[i])&&new_ap.name[i]!=' ')     //If the selected character is not Alphabet AND not Whitespace
            {
                printf("INVALID NAME\n");               //Error message
                break;                                  //End loop: i NOT equal to length of Name
            }
    }while(i!=strlen(new_ap.name)&&!isalpha(new_ap.name[0])); //WRONG INPUT   : 1st Character is not an Alphabet
                                                              //                AND i NOT equal to length of Name : Condition True
                                                              //Loop continues till RIGHT input
    for(i=0;i<strlen(new_ap.name);i++)                //For every character of Name
        if(islower(new_ap.name[i]))                          //If it is Lower case
            new_ap.name[i]=toupper(new_ap.name[i]);                   //Convert it to Upper case

    printf("Airport details being added:\n");
    printf("----------------------------------------\n");
    printf("Airport ID:   %s\n",new_ap.id);
    printf("Airport Name: %s\n",new_ap.name);   //Displaying the details of the record to be removed
    printf("CONFIRM?[Y/N] ");                   //Prompt to Input Confirmation
    fflush(stdin);                              //Flushing STDIN
    scanf("%c",&chk);                           //Input of Confirmation
    if(chk=='y'||chk=='Y')                      //If Yes
    {
        fwrite(&new_ap,sizeof(new_ap),1,f);         //Append the Details of New Airport to AIRPORT.dat
        printf("1 Airport Record ADDED!!\n");        //Message to user
    }
    fclose(f);      //Closing FLIGHT.dat opened in "ab+" mode
    sort_airport(); //Updated AIRPORT.dat is sorted
}
int rem_ap()                                //Function to Remove an Airport record
{
    FILE *f,*f1;
    int i;
    char del[4],chk='\0';                  //chk is assigned a value that can only be changed in case a match is found
    AIRPORT temp;
    f=fopen("AIRPORT.dat","rb");             //Opening AIRPORT.dat in READ-ONLY mode
    if(f==NULL)                             //If error in opening the file
    {
        printf("Error in Opening File!!!"); //Error Message
        return 0;                           //Return to Admin Page
    }
    f1=fopen("TEMP.dat","wb");              //Opening TEMP.dat in WRITE mode as a Temporary File
    printf("Enter the AIRPORT ID: ");               //Prompt to Input ID of AIRPORT record to be removed
    fflush(stdin);                          //Flushing STDIN
    scanf("%s",del);                        //Input of the ID
    for(i=0;i<3;i++)                        //For Every letter of ID, i is set to 3 for valid ID
        if(islower(del[i]))                       //If it is Lower case
            del[i]=toupper(del[i]);                     //Convert it to Upper case
    while(fread(&temp,sizeof(temp),1,f))    //Reading ONE Airport record at a time
    {
        if(strcmp(temp.id,del)!=0)          //If Read ID and the Input ID do NOT match
            fwrite(&temp,sizeof(temp),1,f1);    //Write the Airport Record into TEMP.dat
        else                                //If Read ID and the Input ID match
        {
            printf("Flight details being Removed:\n");
            printf("----------------------------------------\n");
            printf("Airport ID:   %s\n",temp.id);
            printf("Airport Name: %s\n",temp.name);   //Displaying the details of the record to be removed
            printf("CONFIRM?[Y/N] ");       //Prompt to input Confirmation
            fflush(stdin);                  //Flushing STDIN
            scanf("%c",&chk);               //Input of Confirmation
            if(chk=='n'||chk=='N')          //If NO
                fwrite(&temp,sizeof(temp),1,f1);    //Write the Flight Record into TEMP.dat
            else                            //Else, The record is skipped
                printf("1 Airport Record REMOVED!!\n");  //Message to User
        }
    }                       //Loop ends after all the records of AIRPORT.dat are read
    if(chk=='\0')                           //If chk is unchanged
        printf("RECORD NOT FOUND!!!");      //No record found with input ID
    fclose(f);                              //Closing AIRPORT.dat
    fclose(f1);                             //Closing TEMP.dat
    remove("AIRPORT.dat");                  //Deleting the AIRPORT.dat
    rename("TEMP.dat","AIRPORT.dat");       //Renaming TEMP.dat to AIRPORT.dat
}
int sort()                                  //Function to Sort the records in FLIGHT.dat
{
    FILE *f1,*f2;
    FLIGHT *f,temp;
    f=(FLIGHT*)malloc(sizeof(FLIGHT));      //Dynamically allocating sizeof(FLIGHT) bytes to f
    int tot=0,i,j;                          //tot - Variable to store number of Flight records in FLIGHT.dat
    f1=fopen("FLIGHT.dat","rb");            //Opening FLIGHT.dat in READ-ONLY mode
    if(f==NULL)                             //If error in opening the file
    {
        printf("Error in Opening File!!!"); //Error Message
        return 0;                           //Return to Admin Page
    }
    f2=fopen("temp.dat","wb");              //Opening temp.dat in WRITE mode as a temporary file
    while(fread(f,sizeof(FLIGHT),1,f1))     //Reading ONE Flight record at a time
        tot++;                                  //Counting the Flight records
    rewind(f1);                             //Moving f1 to Beginning of File
    free(f);                                //Deallocating the memory allocated to f
    f=(FLIGHT*)calloc(tot,sizeof(FLIGHT));  //Dynamically allocating tot times sizeof(FLIGHT) bytes to f
    fread(f,sizeof(FLIGHT),tot,f1);         //Reading contents of FLIGHT.dat (tot Flight records) into f
    for(i=0;i<tot;i++)                      //Selection Sort of f
    {
        for(j=0;j<i;j++)
        {
            if(strcmp(f[i].id,f[j].id)<0)   //Sorting by Flight ID
            {
                temp=f[i];
                f[i]=f[j];
                f[j]=temp;
            }
        }
    }
    fwrite(f,sizeof(FLIGHT),tot,f2);        //Writing th contents of f into the file temp.dat
    free(f);                                //Deallocating the memory allocated to f
    fclose(f1);                             //Closing FLIGHT.dat
    fclose(f2);                             //Closing temp.dat
    remove("FLIGHT.dat");                   //Deleting FLIGHT.dat
    rename("temp.dat","FLIGHT.dat");        //Renaming temp.dat to FLIGHT.dat
    printf("SORTED!!!\n");                  //Message to User
}
int sort_airport()                          //Function to Sort the records in AIRPORT.dat
{
    FILE *f1,*f2;
    AIRPORT *f,temp;
    f=(AIRPORT*)malloc(sizeof(AIRPORT));    //Dynamically allocating sizeof(AIRPORT) bytes to f
    int tot=0,i,j;                          //tot - Variable to store number of Airport records in AIRPORT.dat
    f1=fopen("AIRPORT.dat","rb");           //Opening AIRPORT.dat in READ-ONLY mode
    if(f==NULL)                             //If Error in opening the file
    {
        printf("Error in Opening File!!!"); //Error message
        return 0;                           //Return to Admin page
    }
    f2=fopen("temp.dat","wb");              //Opening temp.dat in WRITE mode as temporary file
    while(fread(f,sizeof(AIRPORT),1,f1))    //Reading ONE Airport Record at a time
        tot++;                                  //Counting the Airport records
    rewind(f1);                             //Moving f1 to Beginning of file
    free(f);                                //Deallocating the memory allocated to f
    f=(AIRPORT*)calloc(tot,sizeof(AIRPORT));//Dynamically allocating tot times sizeof(AIRPORT) bytes to f
    fread(f,sizeof(AIRPORT),tot,f1);        //Reading contents of AIRPORT.dat (tot Airport records) into f
    for(i=0;i<tot;i++)                      //Selection sort
    {
        for(j=0;j<i;j++)
        {
            if(strcmp(f[i].id,f[j].id)<0)   //Sort by Airport ID
            {
                temp=f[i];
                f[i]=f[j];
                f[j]=temp;
            }
        }
    }
    fwrite(f,sizeof(AIRPORT),tot,f2);       //Writing the contents of f into the file temp.dat
    free(f);                                //Deallocating the memory allocated to f
    fclose(f1);                             //Closing AIRPORT.dat
    fclose(f2);                             //Closing temp.dat
    remove("AIRPORT.dat");                  //Deleting AIRPORT.dat
    rename("temp.dat","AIRPORT.dat");       //Renaming temp.dat to AIRPORT.dat
    return 0;                               //Return to Admin Page
}
void check_airport(char *air)           //Function to Check and Append an AIRPORT ID
{
    AIRPORT a;
    FILE *f1;
    int i;
    f1=fopen("AIRPORT.dat","ab+");      //Opening AIRPORT.dat in APPEND and READ mode
    rewind(f1);                         //Move f1 to beginning of file
    while(fread(&a,sizeof(a),1,f1))     //Reading ONE Airport ID at a time
    {
        if(strcmp(air,a.id)==0)         //If Read ID and Given ID match
            break;                          //f1 is not End of File, Ending the loop
    }
    if(feof(f1))     //End of File if no ID matches
    {
        printf("Airport %s NOT FOUND!!!",air);
        strcpy(a.id,air);       //Given ID is copied into a.id
        do          //Loop to validate input of name
        {
            printf("\nEnter the Name of Airport: ");    //Prompt to input Name
            fflush(stdin);                              //Flushing STDIN
            gets(a.name);                               //Input of Name
            for(i=0;i<strlen(a.name);i++)               //For Every character of Name
                if(!isalpha(a.name[i])&&a.name[i]!=' ')     //If the selected character is not Alphabet AND not Whitespace
                {
                    printf("INVALID NAME\n");               //Error message
                    break;                                  //End loop: i NOT equal to length of Name
                }
        }while(i!=strlen(a.name)&&!isalpha(a.name[0])); //WRONG INPUT   : 1st Character is not an Alphabet
                                                        //                AND i NOT equal to length of Name : Condition True
                                                        //Loop continues till RIGHT input
        for(i=0;i<strlen(a.name);i++)                   //For every character of Name
            if(islower(a.name[i]))                          //If it is Lower case
                a.name[i]=toupper(a.name[i]);                   //Convert it to Upper case
        fwrite(&a,sizeof(a),1,f1);                      //Append the new AIRPORT Details into AIRPORT.dat
        i=30;                           //Assigning i value that cannot be achieved outside this block
    }
    fclose(f1);                         //Closing AIRPORT.dat
    if(i==30)                           //If NO ID matches and New ID created
        sort_airport();                     //Sort the records in AIRPORT.dat
}
void read_time(TIME *t)     //Function to read variable of type TIME
{
    do        //Loop to validate input of DAY
    {
        printf("Enter the Index of Day [1-7]: ");   //Prompt to input Index of DAY
        scanf("%d",&(t->day));                      //Input of DAY
    }while(t->day<1||t->day>7);             //Wrong Input: Condition TRUE: Loop continues
                                            //Right Input: DAY [1-7]: Loop ends
    do        //Loop to validate input of HOUR
    {
        printf("Enter the Hour [0-23]: ");          //Prompt to input Index of HOUR
        scanf("%d",&(t->hour));                     //Input of HOUR
    }while(t->hour<0||t->hour>23);          //Wrong Input: Condition TRUE: Loop continues
                                            //Right Input: HOUR [0-23]: Loop ends
    do        //Loop to validate input of MIN
    {
        printf("Enter the Minute [0-59]: ");        //Prompt to input Index of MIN
        scanf("%d",&(t->min));                      //Input of MIN
    }while(t->min<0||t->min>59);            //Wrong Input: Condition TRUE: Loop continues
                                            //Right Input: HOUR [0-59]: Loop ends
}
