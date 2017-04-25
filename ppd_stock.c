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
#include "ppd_stock.h"
#include "ppd_utility.h"

/*initialize stock list*/
struct ppd_list * init_stock_list()
{
    struct ppd_list * stock_list = malloc(sizeof(* stock_list));
    /*Checks if there is enough memory to create new node*/
    if(stock_list != NULL)
    {
        stock_list->head = NULL;
        stock_list->count = 0;
    }
    return stock_list;
}

/*add stock to the stock list*/
BOOLEAN add_to_stock_list(struct ppd_list * stock_list, 
                            struct ppd_stock * new_stock_data)
{
    struct ppd_node * previous_stock_node;
    struct ppd_node * current_stock_node;

    struct ppd_node * new_stock_node = malloc(sizeof(* new_stock_node));
    /*Not enough free memory*/
    if(new_stock_node == NULL)
    {
        return FALSE;
    }
    new_stock_node->data = malloc(sizeof(* new_stock_node->data));
    /*Not enough free memory of stock data*/
    if(new_stock_node->data == NULL)
    {
        free(new_stock_node);
        return FALSE;
    }
    /*Copies the new stock data being fed into this function into 
    a new_stock_data*/
    * new_stock_node->data = * new_stock_data;
    new_stock_node->next = NULL;

    /*Initializing to start looping from the beginning of the list*/
    previous_stock_node = NULL;
    current_stock_node = stock_list->head;

    /*checks if stock_list is empty*/
    /*executes while loop when it is not empty*/
    while(current_stock_node != NULL)
    {
        /*sorting alphabetically based on name*/
        if(strcmp(new_stock_node->data->name, current_stock_node->data->name)<0)
        {
            break;
        }
        /*If new stock name > current stock name move one step along the stock 
        list*/
        /*override previous stock with the current stock*/
        previous_stock_node = current_stock_node;
        /*current stock becomes the next item on the list*/
        current_stock_node = current_stock_node->next;
    }
    /*Checks if the list is empty*/
    if(stock_list->head == NULL)
    {
        /*Alters the stock_list->head to point to the new_stock*/
        stock_list->head = new_stock_node;
        /*new_stock->next remains the same NULL as previously initialized*/
    }
    /*Checks if the new stock is being sorted at the beginning of the list */
    else if(previous_stock_node == NULL)
    {
        /*new stock will point to what the head pointed to previously*/
        new_stock_node->next = stock_list->head;
        /*make the head point to the new node*/
        stock_list->head = new_stock_node;
    }
    /*Only triggers if it is sorted to the very end of the list*/
    else
    {
        /*previous stock will now point to the new stock*/
        previous_stock_node->next = new_stock_node;
        /*current_stock will be null since it broke out of the while loop 
        naturally*/
        new_stock_node->next = current_stock_node;
    }
    /*increments the stock_list quantity*/
    stock_list->count++;
    return TRUE;
}

/*removes product from linked list*/
void remove_from_stock_list(struct ppd_list * stock_list, 
                            struct ppd_node * previous_stock_node, 
                            struct ppd_node * current_stock_node)
{
    if(previous_stock_node == NULL)
    {
        /*Only triggers if the stock being removed is at the beginning of the 
        list*/
        stock_list->head = current_stock_node->next;
    }
    else
    {
        /*previous stock will now point to the stock being pointed by the 
        removed stock*/
        previous_stock_node->next = current_stock_node->next;
    }
    /*once done, free all the structs involved*/
    free(current_stock_node->data);
    free(current_stock_node);
    /*Reduces the stock_list count*/
    stock_list->count--;
}

/*prints the stock list*/
void print_stock_list(struct ppd_list * stock_list)
{
    struct ppd_node * next_stock_node = stock_list->head;
    int count;
    int longest_name = 0;
    /*first looks for the stock with the longest name*/
    while(next_stock_node != NULL)
    {
        if(strlen(next_stock_node->data->name) > longest_name)
        {
            longest_name = strlen(next_stock_node->data->name);
        }
        next_stock_node = next_stock_node->next;
    }

    printf("Items Menu\n\n");
    printf("ID    | ");
    printf("Name");
    /*Makes the table dynamic in response to the longest name*/
    for(count = strlen("Name"); count < longest_name + 1; count++)
    {
        printf(" ");
    }
    printf("| Available | ");
    printf("Price\n");
    /*formats the '-' line to compensate for longest name*/
    for(count = 0; count < MIN_DASHES + longest_name; count++)
    {
        printf("-");
    }
    printf("\n");
    next_stock_node = stock_list->head;
    /*loops through all the stock again for the second time*/
    /*Prints all the data with the respective formating*/
    while(next_stock_node != NULL)
    {
        printf("%s", next_stock_node->data->id);
        for(count = strlen(next_stock_node->data->id); count < IDLEN + 1; 
            count++)
        {
            printf(" ");
        }
        printf("| ");
        printf("%s", next_stock_node->data->name);
        for(count = strlen(next_stock_node->data->name); 
            count < longest_name + 1; count++)
        {
            printf(" ");
        }
        printf("| ");
        printf("%d", next_stock_node->data->on_hand);
        if(next_stock_node->data->on_hand < 10)
        {
            printf("         ");
        }
        else if(next_stock_node->data->on_hand < 100)
        {
            printf("        ");
        }
        printf("| ");
        printf("$ %d.%02d\n", next_stock_node->data->price.dollars, 
                next_stock_node->data->price.cents);
        next_stock_node = next_stock_node->next; 
    }
}

/*Frees all the data pertaining to the stock list*/
struct ppd_list * free_stock_list(struct ppd_list * stock_list)
{
    struct ppd_node * next_stock_node = stock_list->head;
    /*starts deleting from the head*/
    while(next_stock_node != NULL)
    {
        struct ppd_node * current_stock_node = next_stock_node;
        next_stock_node = next_stock_node->next;

        free(current_stock_node->data);
        free(current_stock_node);
    }
    /*Once all data has been freed, the head of stock list will be freed*/
    free(stock_list);
    return NULL;
}

/*Sets all the stock on hand value to the default value*/
void reset_stock_quantity(struct ppd_system * system)
{
    struct ppd_node * next_stock_node = system->item_list->head;
    while(next_stock_node != NULL)
    {
        next_stock_node->data->on_hand = DEFAULT_STOCK_LEVEL;
        next_stock_node = next_stock_node->next;
    }
    printf("All stock has been reset to the default level of %d.\n", DEFAULT_STOCK_LEVEL);
}

/*generates the numerical component of the stock ID*/
int generate_stock_id_number(struct ppd_system * system)
{
    struct ppd_node * current_stock_node;
    char * id_ptr;
    int current_id_number = 0;
    int generated_id_number = 0;
    current_stock_node = system->item_list->head;
    /*Loops through the whole list to find the largest ID value*/
    while(current_stock_node != NULL)
    {
        /*Since ID is validated during system load and generated by the system
        when adding new products, we do not check if the strtol returns a fully
        numeric conversion*/
        current_id_number = (int) strtol(current_stock_node->data->id + 1, 
                                            &id_ptr, 10);
        if(current_id_number > generated_id_number)
        {
            generated_id_number = current_id_number;
        }
        current_stock_node = current_stock_node->next;
    }
    /*returns the generated ID + 1 to avoid a clash*/
    return generated_id_number + 1;
}

/*Validates user input for stock name*/
BOOLEAN ask_for_stock_name(struct ppd_stock * new_stock_data)
{
    char name[NAMELEN + 2];
    while(TRUE)
    {
        /*Input request and validation check for name */
        printf("Enter the item name: ");
        /*checks if input is null*/
        if(fgets(name, sizeof(name), stdin) == NULL)
        {
            printf("\nCtrl-D detected but not supported.\n");
        }
        /*checks if enter is entered*/
        else if(name[0] == NEWLINE)
        {
            printf("Returning to Main Menu.\n");
            return FALSE;
        }
        /*Checks if user input is too long*/
        else if(name[strlen(name) - 1] != NEWLINE)
        {
            read_rest_of_line();
            printf("Input too long.\n");
        }
        else
        {
            /*Gets rid of newline*/
            name[strlen(name) - 1] = EMPTY;
            /*verifies that there is no stray '|' within the user input*/
            if(check_for_delimiter(name))
            {
                strcpy(new_stock_data->name, name);
                return TRUE;
            }
            else
            {
                printf("A reserve character '|' has been detected. ");
                printf("Name entered is invalid.\n");
            }
        }
    }
}

/*Validates user input for stock desc*/
BOOLEAN ask_for_stock_desc(struct ppd_stock * new_stock_data)
{
    char desc[DESCLEN + 2];
    while(TRUE)
    {
        /*Input request and validation check for description */
        printf("Enter the item description: ");
        /*checks if input is null*/
        if(fgets(desc, sizeof(desc), stdin) == NULL)
        {
            printf("\nCtrl-D detected but not supported.\n");
        }
        /*checks if enter is entered*/
        else if(desc[0] == NEWLINE)
        {
            printf("Returning to Main Menu.\n");
            return FALSE;
        }
        /*Checks if user input is too long*/
        else if(desc[strlen(desc) - 1] != NEWLINE)
        {
            read_rest_of_line();
            printf("Input too long.\n");
        }
        else
        {
            /*Gets rid of newline*/
            desc[strlen(desc) - 1] = EMPTY;
            /*verifies that there is no stray '|' within the user input*/
            if(check_for_delimiter(desc))
            {
                strcpy(new_stock_data->desc, desc);
                return TRUE;
            }
            else
            {
                printf("A reserve character '|' has been detected. ");
                printf("Description entered is invalid.\n");
            }
        }
    }
}

/*Ask user input for stock price*/
BOOLEAN ask_for_stock_price(struct ppd_stock * new_stock_data)
{
    char price[PRICELEN + 2];
    while(TRUE)
    {
        /*Input request and validation check for price*/
        printf("Enter the price of the item: ");
        /*Checks if user input is null*/
        if(fgets(price, sizeof(price), stdin) == NULL)
        {
            printf("\nCtrl-D detected but not supported.\n");
        }
        /*Checks if enter was entered*/
        else if(price[0] == NEWLINE)
        {
            printf("Returning to Main Menu.\n");
            return FALSE;
        }
        /*Checks if the user input is too long*/
        else if(price[strlen(price) - 1] != NEWLINE)
        {
            read_rest_of_line();
            printf("Input too long.\n");
        }
        else
        {
            /*remove newline from user input*/
            price[strlen(price) - 1] = EMPTY;
            /*Validate the user input for price*/
            if(validate_stock_price_input(new_stock_data, price))
            {
                return TRUE;
            }
        }
    }
}

/*Validate user input for stock price*/
BOOLEAN validate_stock_price_input(struct ppd_stock * new_stock_data, 
                                    char * price)
{
    char * price_ints[3];
    char * price_ptr;
    char * dollars_ptr;
    char * cents_ptr;
    
    int count;
    int dollars;
    int cents;
    count = 0;

    /*Tokenize the user input for price*/
    price_ptr = strtok(price, ".");
    while(price_ptr != NULL)
    {
        price_ints[count] = price_ptr;
        count++;
        price_ptr = strtok(NULL, ".");
    }
    /*Triggers when the delimiter is present more than once
    rendering it invalid*/
    if(count != 2)
    {
        printf("The price entered has to have a dollars and cents format.\n");
        return FALSE;
    }
    else
    {
        /*Converts to data to dollars and cents*/
        dollars = (int) strtol(price_ints[0], &dollars_ptr, 10);
        cents = (int) strtol(price_ints[1], &cents_ptr, 10);
        /*CHECKS IF THE INPUTS ARE PURELY NUMERICAL*/
        if(* dollars_ptr != EMPTY || * cents_ptr != EMPTY)
        {
            printf("The price entered has to be numerical.\n");
            return FALSE;
        }
        /*MAX_DOLLARS_VALUE is housed in ppd_stock.h*/
        /*Dollars need to be within 0 and MAX_DOLLARS_VALUE*/
        else if(dollars > MAX_DOLLARS_VALUE || dollars < 0)
        {
            printf("The value entered for dollars cannot exceed %d.\n",
                    MAX_DOLLARS_VALUE);
            return FALSE;
        }
        /*CHECKS IF CENTS ARE MULTIPLES OF 5 AND NOT MORE THAN 95*/
        /*Cents cannot be negative as well*/
        /*MAX_CENTS_VALUE is housed in ppd_stock.h*/
        else if(cents%5 != 0 || cents > MAX_CENTS_VALUE || cents < 0)
        {
            printf("The value entered for cents must be multiples of 5 ");
            printf("and not more than 95.\n");
            return FALSE;
        }
        /*Checks if both dollars and cents are zero*/
        else if(dollars == 0 && cents == 0)
        {
            printf("The product added cannot be free.\n");
            return FALSE;
        }
        /*Adds the data to their respective slots if they pass all validation*/
        else
        {
            new_stock_data->price.dollars = dollars;
            new_stock_data->price.cents = cents;
            return TRUE;
        }
    }
    return FALSE;
}

/*function used to check if there is a '|' in the inputs for name and 
description*/
BOOLEAN check_for_delimiter(char * user_input)
{
    int count;
    char * ptr;
    count = 0;
    /*Tokenize to look for delimiters*/
    /*Could have used strchr instead*/
    ptr = strtok(user_input, "|");
    while(ptr != NULL)
    {
        count++;
        ptr = strtok(NULL, "|");
    }
    /*Count will always start at 1*/
    if(count == 1)
    {
        return TRUE;
    }
    /*Triggers when '|' is found*/
    else
    {
        return FALSE;
    }
}

/*Saves the changes made to stock to a file*/
BOOLEAN save_stock_to_file(struct ppd_system * system)
{
    FILE * stock_file = NULL;
    /*Tries to open the previously loaded stock file*/
    stock_file = (fopen(system->stock_file_name, "w"));
    if(stock_file == NULL)
    {
        printf("Error. Unable to open %s.\n", system->stock_file_name);
        return FALSE;
    }
    /*Loops through all the stock data and writes them to the file*/
    else
    {
        struct ppd_node * next_stock_node = system->item_list->head;
        while(next_stock_node != NULL)
        {
            fprintf(stock_file, "%s|%s|%s|%d.%02d|%d\n", 
                next_stock_node->data->id, 
                next_stock_node->data->name, 
                next_stock_node->data->desc, 
                next_stock_node->data->price.dollars, 
                next_stock_node->data->price.cents, 
                next_stock_node->data->on_hand);
            next_stock_node = next_stock_node->next;
        }
        /*Once all of it has been saved
        Print success message and close the file*/
        printf("Stock details successfully stored as %s.\n", 
                system->stock_file_name);
        fclose(stock_file);
        return TRUE;
    }
}