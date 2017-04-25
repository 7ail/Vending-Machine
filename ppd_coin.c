/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : HAN LIANG ALVIN SEE
 * Student Number   : S3557766
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_coin.h"
#include "ppd_stock.h"
#include "ppd_shared.h"
#include "ppd_utility.h"

/*Initialize cash register to safe values*/
void init_cash_register(struct ppd_system * system)
{
    int i;
    for(i = 0; i < NUM_DENOMS; i++)
    {
        system->cash_register[i].count = 0;
    }
    /*This is hardcoded as I changed the underlying values of the denomination
    enum*/
    system->cash_register[0].denom = FIVE_CENTS;
    system->cash_register[1].denom = TEN_CENTS;
    system->cash_register[2].denom = TWENTY_CENTS;
    system->cash_register[3].denom = FIFTY_CENTS;
    system->cash_register[4].denom = ONE_DOLLAR;
    system->cash_register[5].denom = TWO_DOLLARS;
    system->cash_register[6].denom = FIVE_DOLLARS;
    system->cash_register[7].denom = TEN_DOLLARS;
}

/*prints the cash register*/
void print_cash_register(struct ppd_system * system)
{
    printf("Coins Summary\n");
    printf("---------\n");
    printf("Denomination | Count\n\n");
    printf("5 cents      | %d\n", system->cash_register[0].count);
    printf("10 cents     | %d\n", system->cash_register[1].count);
    printf("20 cents     | %d\n", system->cash_register[2].count);
    printf("50 cents     | %d\n", system->cash_register[3].count);
    printf("1 dollar     | %d\n", system->cash_register[4].count);
    printf("2 dollar     | %d\n", system->cash_register[5].count);
    printf("5 dollar     | %d\n", system->cash_register[6].count);
    printf("10 dollar    | %d\n", system->cash_register[7].count);
}

/*resets the count for all denominations to 20*/
void reset_cash_register(struct ppd_system * system)
{
    int i;
    /*Loops through all denoms*/
    for(i = 0; i < NUM_DENOMS; i++)
    {
        /*Sets all count to default values*/
        system->cash_register[i].count = DEFAULT_COIN_COUNT;
    }
    printf("All coins have been reset to the default level of %d.\n", 
            DEFAULT_STOCK_LEVEL);
}

BOOLEAN save_coins_to_file(struct ppd_system * system)
{
    int count;
    FILE * coin_file = NULL;
    /*Opens a file depending on the name of the previously loaded coin file*/
    coin_file = (fopen(system->coin_file_name, "w"));
    /*Catches the condition where a coin file was not loaded previously*/
    if(coin_file == NULL)
    {
        printf("Error. Unable to open %s.\n", system->coin_file_name);
        return FALSE;
    }
    else
    {
        /*Loops through all denoms*/
        for(count = (NUM_DENOMS - 1); count >= 0; count--)
        {
            /*Prints all relevant data to the file in the required format*/
            fprintf(coin_file, "%d,%d\n", system->cash_register[count].denom, 
                    system->cash_register[count].count);
        }
        printf("Coin details successfully stored as %s.\n", 
                system->coin_file_name);
        /*Close the file once we are done*/
        fclose(coin_file);
        return TRUE;
    }
}

/*Used to generate the list of denoms used when returning money as well as
deduct them from their respective pools*/
BOOLEAN handle_change(struct coin cash_register[], char change[], int total)
{
    /*Creates a temp cash register*/
    struct coin temp_cash_register[NUM_DENOMS];
    int dollars;
    int cents;
    /*Starts the count from the highest denom*/
    int count = NUM_DENOMS -1;
    /*Sets the format of the message being returned*/
    char message[SIZE_OF_BUFFER] = ":";

    /*Copies the content of the main cash register into the temp cash register*/
    memcpy(temp_cash_register, cash_register, sizeof(temp_cash_register));
    /*Starts looping through the denoms*/
    while(count >= 0)
    {
        /*Breaks the loop once the total owned reaches 0*/
        if(total == 0)
        {
            /*Replaces the main cash register with the temporary one*/
            memcpy(cash_register, temp_cash_register,
                    sizeof(temp_cash_register));
            /*Returns the generated message back to the previous function*/
            strcpy(change, message);
            return TRUE;
        }
        /*Checks the total owned against the denom value*/
        else if(total >= temp_cash_register[count].denom)
        {
            /*Checks if the count is not zero*/
            if(temp_cash_register[count].count > 0)
            {
                /*Checks if the denom is in dollars or cents*/
                dollars = temp_cash_register[count].denom/100;
                cents = temp_cash_register[count].denom%100;
                if(dollars != 0)
                {
                    sprintf(message, "%s $%d", message, dollars);
                }
                else
                {
                    sprintf(message, "%s %dc", message, cents);
                }
                /*Subtract denom value from total*/
                total -= temp_cash_register[count].denom;
                temp_cash_register[count].count -= 1;
            }
            /*If the count is 0, we move down the list*/
            else
            {
                count--;
            }
        }
        /*If the total is less, we move down the list*/
        else
        {
            count--;
        }
    }
    return FALSE;
}

/*Handles all payment related processes*/
BOOLEAN make_payment(struct ppd_system * system, struct ppd_node * stock)
{
    int payment;
    int outstanding;
    int total_paid = 0;
    int count;
    char user_input[PAYMENTLEN + 2];
    char * remaining;
    char change[SIZE_OF_BUFFER];

    printf("You have selected \"%s\". This will\n", stock->data->desc);
    printf("cost you $%d.%02d.\n", stock->data->price.dollars,
            stock->data->price.cents);
    printf("Please hand over the money - ");
    printf("type in the value of each note/coin in cents.\n");
    printf("Press enter on a new and empty line to cancel this purchase:\n");
    /*Converts the price to cents*/
    outstanding = (stock->data->price.dollars * 100) + stock->data->price.cents;
    /*Checks if the user has paid the minimum amount*/
    while(outstanding > 0)
    {
        printf("You still need to give us $%d.%02d:", outstanding/100, 
                outstanding%100);
        /*Checks for null input*/
        if(fgets(user_input, sizeof(user_input), stdin) == NULL)
        {
            printf("\nCtrl-D detected but not supported.\n");
        }
        /*Checks for enter (exit condition)*/
        else if(user_input[0] == NEWLINE)
        {
            /*Refunds any money already deposited*/
            handle_change(system->cash_register, change, total_paid);
            printf("I'm sorry you changed your mind. ");
            printf("Here is your refund of $%d.%02d", total_paid/100, 
                    total_paid%100);
            printf("%s\n", change);
            printf("Please come back soon.\n");
            return TRUE;
        }
        /*Checks if the input is too long*/
        else if(user_input[strlen(user_input) - 1] != NEWLINE)
        {
            read_rest_of_line();
            printf("Input too long.\n");
        }
        else
        {
            /*Gets rid of the new line at the end of the user input*/
            user_input[strlen(user_input) - 1] = EMPTY;
            /*Convert user input into an int*/
            payment = (int) strtol(user_input, &remaining, 10);
            if(*remaining != EMPTY)
            {
                printf("Error: A non numeric input was detected.\n");
            }
            else if(payment < 0)
            {
                printf("Error: Negative value detected.\n");
            }
            else
            {
                /*Loops through all denoms*/
                for(count = 0; count < NUM_DENOMS; count++)
                {
                    /*Checks if the denom input is valid*/
                    if(payment == system->cash_register[count].denom)
                    {
                        system->cash_register[count].count += 1;
                        outstanding -= payment;
                        total_paid += payment;
                        break;
                    }
                }
                /*If it is not found, repromts a user input*/
                /*No changes are made*/
                if(count == NUM_DENOMS)
                {
                    printf("Error: $%d.%02d ", payment/100, payment%100);
                    printf("is not a valid denomination of money.\n");
                }
            }
        }
    }
    /*Converts change to a positive value*/
    outstanding = outstanding * - 1;
    /*Tries to return the change owed*/
    if(handle_change(system->cash_register, change, outstanding))
    {
        printf("Thank you. Here is your %s, and your change of $%d.%02d", 
                stock->data->name, outstanding/100, outstanding%100);
        printf("%s\n", change);
        printf("Please come back soon.\n");
        stock->data->on_hand -= 1;
        return TRUE;
    }
    /*Refunds whatever the user has paid if an exact change cant be generated*/
    /*handle_change should never return false here*/
    else
    {
        printf("Sorry we are unable to provide change for this transaction.\n");
        handle_change(system->cash_register, change, total_paid);
        printf("The %s will have to wait. Here is your refund of $%d.%02d", 
                stock->data->name, total_paid/100, total_paid%100);
        printf("%s\n", change);
        printf("Please come back soon.\n");
        return TRUE;
    }
    return FALSE;
}