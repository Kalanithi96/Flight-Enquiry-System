#ifndef COMMON      //To Avoid Multiple inclusions of common.h
#define COMMON

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<ctype.h>
typedef struct{     //Structure for USER ID & Password
    char id[20];    //USER ID
    char pw[20];    //USER PASSWORD
}LOGIN;
typedef struct{     //Structure for Airport ID and Name
    char id[4];     //AIRPORT ID
    char name[20];  //AIRPORT NAME
}AIRPORT;
typedef struct{     //Structure for Time
    int day;        //DAY
    int hour;       //HOUR
    int min;        //MINUTE
}TIME;
typedef struct{     //Structure for details of a FLIGHT
    char id[20];    //FLIGHT ID
    char name[20];  //FLIGHT NAME
    char from[4];   //SOURCE AIRPORT ID
    char to[4];     //DESTINATION AIRPORT ID
    TIME arrival;   //TIME OF ARRIVAL AT DESTINATION
    TIME departure; //TIME OF DEPARTURE FROM SOURCE
}FLIGHT;
void adm(int (*act)())      //Callback function for USER OPERATIONS
{
    (*act)();
}
void display_flight(FLIGHT*);   //Function to Display Details of ONE FLIGHT
void disp_day(int);             //Function to Display a DAY depending on its INDEX
void cal_duration(FLIGHT);      //Function to Calculate and Display the Duration of Flight
int display_airports();         //Function to Display ID and Name of all the AIRPORTS
void air_name(char*);           //Function to Display the NAME of Airport Depending on Airport ID
void input_pw(char *pw)         //Function to Input Password
{
    char c;
    int index=0;
    printf("Enter Password: "); //Prompt to Input Password
    fflush(stdin);              //Flushing STDIN
    while((c=getch())!=13       //13- ASCII Code for ENTER KEY '\n'
            &&  index<19)       //Maximum Password Length is 19
    {                           //Until Enter key is entered or input reaches  19 Characters
        if(index<0)
            index=0;
        if(c==8)                //8 - ASCII Code for BACKSPACE '\b'
        {
            printf("\b \b");    //To Delete an '*' when USER enters Backspace
            index--;            //Decrementing the index
            continue;
        }
        pw[index++]=c;          //Input Character stored in the "index" position and index is incremented
        printf("*");            //'*' is printed in place of input character
    }
    pw[index]='\0';                 //Appending '\0' to mark end of string
}
int dis_all_flights()           //Function to Display Details of all Flights
{
    int n,i,tot=0,page=0,ch;    /*tot  - Total number of Flights
                                  page - Page number
                                  n    - Number of Flights per Page */
    FILE *f;                    //File pointer to Open FLIGHT.dat
    do
    {
        printf("Enter the number of flights per Page [1-10]: ");    //Prompt to input n ( MIN:1 , MAX:10 )
        scanf("%d",&n);         //Input of n
        printf("\n");
    }while(n<1||n>10);          //Validation of n
    f=fopen("FLIGHT.dat","rb"); //Opening FLIGHT.dat in READ-ONLY mode
    if(f==NULL)                 //If Error in opening the file
    {
        printf("Error in Opening File!!!");
        return 0;
    }
    FLIGHT *dis;
    dis=(FLIGHT *)malloc(sizeof(FLIGHT)); //Dynamic Allocation of (sizeof(FLIGHT)) bytes to dis
    while(fread(dis,sizeof(FLIGHT),1,f))  //Reading ONE FLIGHT at a time until end of file
        tot++;                            //Counting the number of Flights
    free(dis);                            //Deallocating the memory held by dis
    dis=(FLIGHT*)calloc(n,sizeof(FLIGHT));//Fresh Allocation of "n" times (sizeof(FLIGHT)) bytes to dis
    do
    {
        fseek(f,sizeof(FLIGHT)*n*page,0);   //Moving f to Beginning of the Page "page"
        fread(dis,sizeof(FLIGHT),((page==tot/n)?(tot%n):n),f);  //Last Page when page = total records/number of records per page
                                                                //Number of records in Last Page is tot%n
        for(i=0;i<((page==tot/n)?tot%n:n);i++)
        {
            display_flight(dis+i);              //Displaying ith record in Page "page"
            printf("-------------------------------------------------------\n");
        }
    //Navigating through pages:
        if(tot>n)       //If more than 1 page Exists
        {
            int last=(page==((tot/n)+((tot%n==0)?-1:0)));        //last - 1 if in Last page, 0 if not in Last Page
            do
            {
                if(page==0)                                                                 //If in 1st Page
                    printf("\t1. Next Page\n\t2. End Display");                             //Can Move to Next page or End
                else if(last)                                               //If in Last Page
                                                                            //If n divides tot, Last page is (tot/n)-1
                                                                            //Else, Last page is tot/n
                        printf("\t1. Previous Page\n\t2. End Display");                     //Can Move to Previous Page or end
                    else                                                                    //If Neither First nor Last Page
                        printf("\t1. Previous Page\n\t2. Next Page\n\t3. End Display");     //Can move to Next or Previous page or End
                printf("\nChoose: ");       //Prompt to input Choice
                scanf("%d",&ch);            //Input of Choice ch
                printf("\n");
            }while(  (( last||(page==0))&&(ch<1||ch>2))        //If First or Last page, Validating ch: 1 or 2
                   ||((!last||(page==0))&&(ch<1||ch>3)));      //Else, Validating ch: 1 or 2 or 3
            switch(ch)
            {
                case 1: (last)?page--:(page==0)?page++:page--;  //If First Page, Next page: page++
                                                                //If Last Page , Previous page: page--
                                                                //Else Previous page: page--
                        break;
                case 2: if(last||(page==0))                    //If in First or Last Page, End display
                        {
                            fclose(f);                          //Closing f
                            free(dis);                          //Deallocating dis
                            return 0;
                        }
                        else                                    //Else, Next page: page++
                            page++;
                        break;
                case 3: fclose(f);                              //Closing f
                        free(dis);                              //Deallocating dis
                        return 0;                               //End display
            }
        }
        else                //If Only one Page exists, No Navigation possible
        {
            fclose(f);      //Closing f
            free(dis);      //Deallocating dis
            return 0;       //End Display
        }
    }while(1);              //End of display only when User Chooses to
}
void display_flight(FLIGHT *f)              //Function to Display details of ONE Flight
{
    printf("Flight ID:\t%s\n",f->id);       //Displaying Flight ID
    printf("Flight Name:\t%s\n",f->name);   //Displaying Flight Name
    printf("From:");
    air_name(f->from);                      //Displaying Source Airport Name
    printf("\nTo  :");
    air_name(f->to);                        //Displaying Destination Airport Name
    printf("\nDeparture: ");
    disp_day(f->departure.day);             //Displaying Departure Day
    printf("\t%02d:%02d\n",f->departure.hour,f->departure.min); //Displaying Departure Time
    printf("Arrival:   ");
    disp_day(f->arrival.day);               //Displaying Arrival Day
    printf("\t%02d:%02d\n",f->arrival.hour,f->arrival.min);     //Displaying Arrival Time
    cal_duration(*f);                       //Calculating and Displaying Duration of Flight
}
void disp_day(int day)     //Function to display Day depending on its Index
{
     /* 1 - SUNDAY
        2 - MONDAY
        3 - TUESDAY
        4 - WEDNESDAY
        5 - THURSDAY
        6 - FRIDAY
        7 - SATURDAY*/
    switch(day)
    {
        case 1: printf("SUNDAY");
                break;
        case 2: printf("MONDAY");
                break;
        case 3: printf("TUESDAY");
                break;
        case 4: printf("WEDNESDAY");
                break;
        case 5: printf("THURSDAY");
                break;
        case 6: printf("FRIDAY");
                break;
        case 7: printf("SATURDAY");
                break;
    }
}
void cal_duration(FLIGHT f)     //Function to Calculate and Display Duration of Flight
{
    TIME t, arr=f.arrival,
            dep=f.departure;
    if(arr.min==dep.min   &&
       arr.hour==dep.hour &&
       arr.day==dep.day)                //IF arr is equal to dep
        t.min=t.hour=0,t.day=7;
    else                                //IF arr NOT equal to dep
    {
        if(arr.min<dep.min)             //IF Arrival Minute < Departure Minute
        {
            dep.hour++;                 //Departure Hour is incremented
            t.min=arr.min-dep.min+60;   //Duration Minute(s) is Arrival Minute - Departure Minute + 60 minutes
        }
        else                            //IF Arrival Minute >= Departure Minute
            t.min=arr.min-dep.min;      //Duration Minute(s) is Arrival Minute - Departure Minute

        if(arr.hour<dep.hour)           //IF Arrival Hour < Departure Hour
        {
            dep.day++;                  //Departure Day is incremented
            t.hour=arr.hour-dep.hour+24;//Duration Hour(s) is Arrival Hour - Departure Hour + 24 Hours
        }
        else                            //IF Arrival Hour >= Departure Hour
            t.hour=arr.hour-dep.hour;   //Duration Hour(s) is Arrival Hour - Departure Hour

        if(arr.day<dep.day)             //IF Arrival Day < Departure Day
            t.day=arr.day-dep.day+7;    //Duration Day(s) is Arrival Day - Departure Day + 7 Days
        else                            //IF Arrival Day >= Departure Day
            t.day=arr.day-dep.day;      //Duration Day(s) is Arrival Day - Departure Day
    }
    printf("\nDURATION: %d Day(s), %02d Hour(s) & %02d Minute(s)\n",t.day,t.hour,t.min);
    //Displaying the Duration of Flight
}
int display_airports()      //Function to Display ID and Name of all the AIRPORTS
{
    AIRPORT a;
    FILE *f1;                               //File Pointer f1 to open AIRPORT.dat
    f1=fopen("AIRPORT.dat","rb");           //Opening AIRPORT.dat in READ-ONLY mode
    printf("\nLIST OF AIRPORTS:");
    printf("\nCode | NAME");
    printf("\n-----+----------------\n");
    while(fread(&a,sizeof(a),1,f1))         //Reading Details of Airports One at a time
        printf("%4s | %s\n",a.id,a.name);   //Displaying Details of Airports One at a time
    printf("\n");
    fclose(f1);                             //Closing the file
    return 0;
}
void air_name(char *air)            //Function to Display the NAME of Airport Depending on Airport ID
{
    FILE *f;                        //File Pointer f to open AIRPORT.dat
    AIRPORT a;
    f=fopen("AIRPORT.dat","rb");    //Opening AIRPORT.dat in READ-ONLY mode
    while(fread(&a,sizeof(a),1,f))  //Reading Details of Airports One at a time
        if(strcmp(a.id,air)==0)     //If the read Airport ID is same as air
        {
            printf("\t%s  -\t%s",a.id,a.name);  //Display the details of Airport
            break;
        }
    fclose(f);                      //Closing f
}

#endif // COMMON
