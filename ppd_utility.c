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

#include "ppd_utility.h"

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!=NEWLINE && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/*Sets all system values to safe values*/
BOOLEAN system_init(struct ppd_system * system)
{
    /*Sets all count to zero and fills in the denom fields*/
    init_cash_register(system);
    /*Allocates the needed memory and set them to safe values*/
    system->item_list = init_stock_list();
    /*Checks if it failed*/
    if(system->item_list == NULL)
    {
        printf("Error. Unable to allocate memory\n");
        return FALSE;
    }
    system->coin_file_name = NULL;
    system->stock_file_name = NULL;
    system->coin_from_file = FALSE;
    return TRUE;
}

/*Used to load stock when the program boots up*/
BOOLEAN load_stock(struct ppd_system * system, const char * stock_file_name)
{
    char buffer[SIZE_OF_BUFFER];
    char id[IDLEN];
    char name[NAMELEN];
    char desc[DESCLEN];

    char * str_remainder;
    char * token;

    int count;
    int dollars;
    int cents;
    int on_hand;
    
    FILE * stock_file = NULL;
    /*saves the file name being processed*/
    system->stock_file_name = stock_file_name;
    /*Opens the file*/
    stock_file = (fopen(stock_file_name, "r"));
    /*Makes sure that it opens successfully*/
    if(stock_file == NULL)
    {
        printf("Error. Unable to open %s.\n", system->stock_file_name);
        return FALSE;
    }
    /*Reads the file line by line*/
    while(fgets(buffer, sizeof(buffer), stock_file) != NULL)
    {
        /*Checks that the amount of tokens in a single line valid*/
        if(!stock_file_token_validation(buffer, stock_file_name))
        {
            fclose(stock_file);
            return FALSE;
        }
        token = strtok(buffer, "|");
        /*validates the data being tokenized*/
        if(!id_validation(token))
        {
            fclose(stock_file);
            return FALSE;
        }
        /*checks for duplicates*/
        if(!check_for_duplicate_id(system->item_list, token))
        {
            fclose(stock_file);
            return FALSE;
        }
        /*copies the token into a more permanent storage*/
        strcpy(id, token);
        /*Loops through all the remaining tokens*/
        for(count = 0; count < 5; count++)
        {
            /*Depending on the token count, different delimiter is used*/
            /*Used for price*/
            if(count == 2)
            {
                token = strtok(NULL, ".");
                /*Checks if the token is valid*/
                if(token == NULL)
                {
                    printf("Invalid token.\n");
                    fclose(stock_file);
                    return FALSE;
                }
            }
            /*Used fror the rest*/
            else
            {
                token = strtok(NULL, "|");
                /*Checks if the token is valid*/
                if(token == NULL)
                {
                    printf("Invalid token.\n");
                    fclose(stock_file);
                    return FALSE;
                }
            }

            /*Depending on the count, the data being saved is different*/
            if(count == 0)
            {
                /*Checks if the token corresponding to name does not exceed the
                max length alocated for the name variable*/
                if(strlen(token) > NAMELEN)
                {
                    /*Stops processing if any error occurs*/
                    printf("Invalid Product Name: Length exceeds %d.\n", 
                            NAMELEN);
                    fclose(stock_file);
                    return FALSE;
                }
                strcpy(name, token);
            }
            /*Same test is made for description (Only the max length is 
            different)*/
            else if(count == 1)
            {
                if(strlen(token) > DESCLEN)
                {
                    printf("Invalid Product Description: Length exceeds %d.\n", 
                            DESCLEN);
                    fclose(stock_file);
                    return FALSE;
                }
                strcpy(desc, token);
            }
            else if(count == 2)
            {
                dollars = (unsigned int) strtol(token, &str_remainder, 10);
                /*Test if the token corresponding to dollars is fully numeric
                and within bounds*/
                if(!dollar_validation(dollars, str_remainder))
                {
                    fclose(stock_file);
                    return FALSE;
                }
            }
            else if(count == 3)
            {
                cents = (unsigned int) strtol(token, &str_remainder, 10);
                /*Test if the token corresponding to cents is fully numeric
                and within bounds*/
                if(!cent_validation(cents, str_remainder))
                {
                    fclose(stock_file);
                    return FALSE;
                }
            }
            else
            {
                on_hand = (unsigned int) strtol(token, &str_remainder, 10);
                /*Test if the token corresponding to on_hand is fully numeric
                and within bounds*/
                if(!quantity_validation(on_hand, str_remainder))
                {
                    fclose(stock_file);
                    return FALSE;
                }
                /*Tries to add the item to the linked list*/
                if(!add_to_item_list(system->item_list, id, name, desc, dollars, 
                                        cents, on_hand))
                {
                    /*closes file and exits program if it fails*/
                    fclose(stock_file);
                    return FALSE;
                }
            }
        }
    }
    /*if the whole file is read properly, close the file and return TRUE*/
    fclose(stock_file);
    return TRUE;
}

/*Used to load coins when the program boots up*/
BOOLEAN load_coins(struct ppd_system * system, const char * coin_file_name)
{
    char buffer[SIZE_OF_BUFFER];
    int denom;
    int quantity;
    int count = 0;
    char * token;
    char * str_remainder;

    FILE * coin_file = NULL;
    /*saves the file name being processed*/
    system->coin_file_name = coin_file_name;
    /*Tries to open the file*/
    coin_file = (fopen(coin_file_name, "r"));
    /*Checks if it failed*/
    if(coin_file == NULL)
    {
        printf("Error. Unable to open %s.\n", system->coin_file_name);
        return FALSE;
    }
    else
    {
        /*Reads each line one at a time*/
        while(fgets(buffer, sizeof(buffer), coin_file) != NULL)
        {
            count++;
            if(count > NUM_DENOMS)
            {
                printf("Invalid file, coin file consist of more than 7 lines\n");
                fclose(coin_file);
                return FALSE;
            }
            /*Checks that the amount of tokens in a single line valid*/
            if(!coin_file_token_validation(buffer, coin_file_name))
            {
                fclose(coin_file);
                return FALSE;
            }
            else
            {
                /*Tries to tokenize the denom*/
                token = strtok(buffer, COIN_DELIM);
                if(token == NULL)
                {
                    printf("Denomination not found.\n");
                    fclose(coin_file);
                    return FALSE;
                }
                /*Converts the denom into an int*/
                denom = (int) strtol(token, &str_remainder, 10);
                /*Makes sure the denom is fully numeric and valid*/
                if(denomination_validation(system->cash_register, 
                                            denom, str_remainder))
                {
                    /*Tries to tokenize the denom*/
                    token = strtok(NULL, COIN_DELIM);
                    if(token == NULL)
                    {
                        printf("Quantity not found.\n");
                        fclose(coin_file);
                        return FALSE;
                    }
                    /*Converts the denom into an int*/
                    quantity = (int) strtol(token, &str_remainder, 10);
                    /*Makes sure the denom is fully numeric and valid*/
                    if(quantity_validation(quantity, str_remainder))
                    {
                        /*Sorts the new data into the cash register*/
                        sort_coins(system->cash_register, denom, quantity);
                    }
                    /*ends the process if it is not fully numeric or valid*/
                    else
                    {
                        fclose(coin_file);
                        return FALSE;
                    }
                }
                /*ends the process if it is not fully numeric or valid*/
                else
                {
                    fclose(coin_file);
                    return FALSE;
                }
            }
        }
        /*if the whole file is read properly, close the file and return TRUE*/
        fclose(coin_file);
        return TRUE;
    }
}

/*Sorts the coins into the cash register*/
/*Validation was already done in denomination_validation()*/
void sort_coins(struct coin cash_register[], int denom, int quantity)
{
    int count;
    /*Search for the right array cell*/
    for(count = 0; count < NUM_DENOMS; count++)
    {
        /*Overrides the count value*/
        if(denom == cash_register[count].denom)
        {
            cash_register[count].count = quantity;
            break;
        }
    }
}

/*frees the linked list*/
void system_free(struct ppd_system * system)
{
    free_stock_list(system->item_list);
}

/*Intermediate step used before calling add_to_stock_list().
Used when adding products once the system is booted up*/
BOOLEAN add_to_item_list(struct ppd_list * item_list, char id[], char name[], 
                            char desc[], int dollars, int cents, int on_hand)
{
    /*Allocate memory for the process*/
    struct ppd_stock * new_stock_data = malloc(sizeof(* new_stock_data));
    /*Check if it failed*/
    if(new_stock_data == NULL)
    {
        printf("Unable to allocate the necessary memory to finish ");
        printf("the process\n");
        return FALSE;
    }
    /*Copies all the new stock data to the temp ppd_stock struct*/
    strcpy(new_stock_data->id, id);
    strcpy(new_stock_data->name, name);
    strcpy(new_stock_data->desc, desc);
    new_stock_data->price.dollars = dollars;
    new_stock_data->price.cents = cents;
    new_stock_data->on_hand = on_hand;
    /*Tries to add the temp struct to the linkedlist*/
    if(!add_to_stock_list(item_list, new_stock_data))
    {
        /*free the temp malloc and return false if it fails*/
        free(new_stock_data);
        return FALSE;
    }
    /*free the temp malloc and return true if it succeeds*/
    free(new_stock_data);
    return TRUE;
}

/*ID validation check*/
BOOLEAN id_validation(char token[])
{
    /*checks if token is null*/
    if(token == NULL)
    {
        printf("ID not found.\n");
        return FALSE;
    }
    /*checks if ID is of length 5*/
    else if(strlen(token) != IDLEN)
    {
        printf("Invalid ID: Length must equal %d.\n", IDLEN);
        return FALSE;
    }
    /*checks if it starts with I*/
    else if(token[0] != 'I')
    {
        printf("Invalid ID: ID must start with 'I'.\n");
        return FALSE;
    }
    /*checks if all the trailing characters are digits*/
    else if(!isdigit(token[1]) || !isdigit(token[2]) || !isdigit(token[3]) || 
            !isdigit(token[4]))
    {
        printf("Invalid ID: ID does not consist of 4 trailing digits.\n");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*Cent value validation check*/
BOOLEAN cent_validation(int cents, char * str_remainder)
{
    /*checks if the data being read is fully numeric*/
    if(* str_remainder != EMPTY)
    {
        printf("Invalid Cent Value: A non numeric was detected.\n");
        return FALSE;
    }
    /*checks if the cents value is within the bounds of 0 and 95*/
    else if(cents > MAX_CENTS_VALUE || cents < 0)
    {
        printf("Invalid Cent Value: Cent is capped at %dc.\n", MAX_CENTS_VALUE);
        return FALSE;
    }
    /*Checks that cents is a factor of 5*/
    else if(cents % 5 != 0)
    {
        printf("Invalid Cent Value: Cent must be divisible by 5.\n");
        return FALSE;
    }
    /*returns true if it passes all test*/
    else
    {
        return TRUE;
    }
}

/*Dollar value validation check*/
BOOLEAN dollar_validation(int dollars, char * str_remainder)
{
    /*checks if the data being read is fully numeric*/
    if(* str_remainder != EMPTY)
    {
        printf("Invalid Dollar Value: A non numeric was detected.\n");
        return FALSE;
    }
    /*checks if the cents value is within the bounds of 0 and 99*/
    else if(dollars > MAX_DOLLARS_VALUE || dollars < 0)
    {
        printf("Invalid Dollar Value: Dollar values are capped at $%d.\n", 
                MAX_DOLLARS_VALUE);
        return FALSE;
    }
    /*returns true if it passes all test*/
    else
    {
        return TRUE;
    }
}

/*Validating the amount of tokens per line in the coin file*/
BOOLEAN coin_file_token_validation(char buffer[], const char * coin_file_name)
{
    int count = 0;
    int number_of_expected_delimiter = 1;
    char * ptr = strchr(buffer, ',');
    /*Checks for recurring delimiters in the string*/
    while(ptr != NULL)
    {
        ptr = strchr(ptr + 1,',');
        count += 1;
    }
    /*if it does not equal the expected value
    print error message and return false*/
    if(count != number_of_expected_delimiter)
    {
        printf("Too many delimiters found in %s.\n", coin_file_name);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*Validating the amount of tokens per line in the stock file
Same logic as coin_file_token_validation()*/
BOOLEAN stock_file_token_validation(char buffer[], const char * stock_file_name)
{
    int count = 0;
    int number_of_expected_delimiter = 4;
    char * ptr = strchr(buffer, '|');
    /*Checks for recurring delimiters in the string*/
    while(ptr != NULL)
    {
        ptr=strchr(ptr + 1,'|');
        count += 1;
    }
    /*if it does not equal the expected value
    print error message and return false*/
    if(count != number_of_expected_delimiter)
    {
        printf("Too many delimiters found in %s.\n", stock_file_name);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*Denomniation validation check*/
BOOLEAN denomination_validation(struct coin cash_register[], 
                                int denom, char * str_remainder)
{
    int count;
    /*checks if token is null*/
    if(* str_remainder != EMPTY)
    {
        printf("Invalid Denomination: A non numeric was detected.\n");
        return FALSE;
    }
    /*Checks if the denomination is valid*/
    for(count = 0; count < NUM_DENOMS; count++)
    {
        if(denom == cash_register[count].denom)
        {
            return TRUE;
        }
    }
    /*This section of code only runs if the denomination is invalid*/
    printf("Invalid Denomination: %d is not a valid denomination.\n", denom);
    return FALSE;
}

/*Quantity value validation check*/
BOOLEAN quantity_validation(int quantity, char * str_remainder)
{
    /*Checks for newline since quantity are both at the end of the string*/
    if(* str_remainder != NEWLINE)
    {
        printf("Invalid Quantity Value: New line character was not detected ");
        printf("or a non numeric was detected.\n");
        return FALSE;
    }
    /*Checks that quantity is within 0 and 99*/
    else if(quantity > MAX_QUANTITY_VALUE || quantity < 0)
    {
        printf("Invalid Quantity Value: Quantity is capped at %d.\n", 
                MAX_QUANTITY_VALUE);
        return FALSE;
    }
    /*returns true if it passes all the test*/
    else
    {
        return TRUE;
    }
}

/*checks for duplicate ID*/
BOOLEAN check_for_duplicate_id(struct ppd_list * item_list, char id[])
{
    struct ppd_node * current_node;
    current_node = item_list->head;
    while(current_node != NULL)
    {
        if(strcmp(id, current_node->data->id) == 0)
        {
            printf("Invalid ID: A duplicate has been found.\n");
            return FALSE;
        }
        current_node = current_node->next;
    }
    return TRUE;
}
