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

#include "ppd_options.h"
#include "ppd_utility.h"
#include "ppd_stock.h"

/*Prints product list*/
BOOLEAN display_items(struct ppd_system * system)
{
    /*Function housed under ppd_stock.c*/
    print_stock_list(system->item_list);
    return TRUE;
}

/*Used to purchase items*/
BOOLEAN purchase_item(struct ppd_system * system)
{
    struct ppd_node * stock;
    char id[IDLEN + 2];
    printf("Purchase Item\n");
    printf("--------------\n");
    while(TRUE)
    {
        printf("Please enter the id of the item you wish to purchase:");
        /*Ask for user input and validates it*/
        /*checks if the input is NULL*/
        if(fgets(id, sizeof(id), stdin) == NULL)
        {
            printf("\nCtrl-D detected but not supported.\n");
        }
        /*Checks if user clicks enter*/
        else if(id[0] == NEWLINE)
        {
            /*Returns user to main menu*/
            printf("Returning to Main Menu.\n");
            return TRUE;
        }
        /*Checks if the user input exceeds the maximum length of an ID*/
        else if(id[strlen(id) - 1] != NEWLINE)
        {
            read_rest_of_line();
            printf("Input too long.\n");
        }
        else
        {
            /*Gets rid of the new line at the end of user input*/
            id[strlen(id) - 1] = EMPTY;
            /*Starts the loop from the beginning*/
            stock = system->item_list->head;
            while(stock != NULL)
            {
                /*breaks out of the search when an item with same id is found*/
                if(strcmp(stock->data->id, id) == 0)
                {
                    break;
                }
                stock = stock->next;
            }
            /*Only runs if a the stock is found*/
            if(stock != NULL)
            {
                /*Checks if there is still stock in hand*/
                if(stock->data->on_hand > 0)
                {
                    /*calls the make_payment function*/
                    /*located in ppd_coins.c*/
                    return make_payment(system, stock);
                }
                /* if count == 0, print error message and returns to
                start of this function*/
                else
                {
                    printf("%s is out of stock.\n", stock->data->name);
                }
            }
            /*if stock cant be found, print error message and returns to start
            of this function*/
            else
            {
                printf("Stock with such ID does not exist.\n");
            }
        }
    }
    return FALSE;
}

/*Called to save any changes made during the session*/
BOOLEAN save_system(struct ppd_system * system)
{
    /*Tries to save the changes made to stock*/
    if(save_stock_to_file(system))
    {
        /*Checks if a coin file was loaded at the start of the session*/
        if(system->coin_from_file)
        {
            /*Tries to save the changes made to coins*/
            if(save_coins_to_file(system))
            {
                printf("All changes has been saved successfully.\n");
                return abort_program();
            }
            /* Returns TRUE if it failed (This is to abort the exit sequence)*/
            else
            {
                printf("An error has occurred when trying to save changes ");
                printf("made to coins\n");
                printf("Exit sequence aborted. ");
                printf("Please manually exit the program.\n");
                return TRUE;
            }
        }
        /*Informs the user that only the changes to stock was saved*/
        else
        {
            printf("Changes made to the stock details were saved.\n");
            printf("A coin file was not loaded for this session.\n");
            printf("Any changes to coin details were not saved.\n");
            return abort_program();
        }
    }
    /*Returns TRUE if it failed (This is to abort the exit sequence)*/
    else
    {
        printf("An error has occurred when trying to save changes made to ");
        printf("stock\n");
        printf("Exit sequence aborted. Please manually exit the program.\n");
        return TRUE;
    }
    return FALSE;
}

/*Called to add new products to the linked list*/
BOOLEAN add_item(struct ppd_system * system)
{
    int id = 0;
    char new_id[IDLEN];
    struct ppd_stock * new_stock_data = malloc(sizeof(* new_stock_data));
    /*Check to see if malloc was successful*/
    if(new_stock_data == NULL)
    {
        printf("Unable to allocate memory to store data.\n");
        return TRUE;
    }

    /*Function located in ppd_stock.c*/
    /*Generates the int portion of the id number*/
    id = generate_stock_id_number(system);
    if(id > LASTID)
    {
        printf("ID generated exceeds %d.\n", LASTID);
        printf("New products cannot be added anymore.\n");
        free(new_stock_data);
        return TRUE;        
    }
    /*turns the value into a valid id*/
    sprintf(new_id, "I%04d", id);
    /*Copies the ID to temporary stock*/
    strcpy(new_stock_data->id, new_id);
    printf("The new meal item will have a the Item id of %s.\n", 
            new_stock_data->id);
    /*Function located in ppd_stock.c*/
    /*Handles the user input and checks for stock name*/
    if(ask_for_stock_name(new_stock_data))
    {
        /*Function located in ppd_stock.c*/
        /*Handles the user input and checks for stock desc*/
        if(ask_for_stock_desc(new_stock_data))
        {
            /*Function located in ppd_stock.c*/
            /*Handles the user input and checks for stock price*/
            if(ask_for_stock_price(new_stock_data))
            {
                /*Sets the stock level to default level*/
                new_stock_data->on_hand = DEFAULT_STOCK_LEVEL;
                /*Tries to add stock to linked list*/
                if(add_to_stock_list(system->item_list, new_stock_data))
                {
                    printf("This item \"%s - %s\" has now been added to the menu.\n",
                        new_stock_data->name, new_stock_data->desc);
                    /*frees the temp malloc created in this function*/
                    free(new_stock_data);
                    return TRUE;
                }
                /*if it failes, print error message and free temp malloc*/
                else
                {
                    printf("Unable to allocate memory to store data.\n");
                    free(new_stock_data);
                    return TRUE;
                }
            }
            /*if user decides to quit, free temp malloc*/
            else
            {
                free(new_stock_data);
                return TRUE;
            }
        }
        /*if user decides to quit, free temp malloc*/
        else
        {
            free(new_stock_data);
            return TRUE;
        }
    }
    /*if user decides to quit, free temp malloc*/
    else
    {
        free(new_stock_data);
        return TRUE;
    }
    return FALSE;
}

/*Used to remove item from the linked list*/
BOOLEAN remove_item(struct ppd_system * system)
{
    char id[IDLEN + 2];
    struct ppd_node * previous_stock_node;
    struct ppd_node * current_stock_node;
    while(TRUE)
    {
        /*Ask for ID of the item that the user wants to remove*/
        printf("Enter the item id of the item to remove from the menu:");
        /*Checks for null*/
        if(fgets(id, sizeof(id), stdin) == NULL)
        {
            printf("\nCtrl-D detected but not supported.\n");
        }
        /*checks if enter was called*/
        else if(id[0] == NEWLINE)
        {
            printf("Returning to Main Menu.\n");
            return TRUE;
        }
        /*Checks if the input is too long*/
        else if(id[strlen(id) - 1] != NEWLINE)
        {
            read_rest_of_line();
            printf("Input too long.\n");
        }
        else
        {
            /*Removes the new line from user input*/
            id[strlen(id) - 1] = EMPTY;
            /*Sets up the looping requirements*/
            previous_stock_node = NULL;
            /*Starts at the head of the linked list*/
            current_stock_node = system->item_list->head;
            /*keeps going until the end of list is found*/
            while(current_stock_node != NULL)
            {
                /*breaks out of the while loop if a match is found*/
                if(strcmp(id, current_stock_node->data->id) == 0)
                {
                    break;
                }
                /*If stock id provided != stock id stored, move one step along 
                the stock list*/
                /*override previous stock with the current stock*/
                previous_stock_node = current_stock_node;
                /*current stock becomes the next item on the list*/
                current_stock_node = current_stock_node->next;
            }
            /*Condition triggers if current stock node is not at the end of the
            list*/
            if(current_stock_node != NULL)
            {
                /*Prints the success message*/
                printf("\"%s - %s %s\" has been removed from the system.\n", 
                        current_stock_node->data->id, 
                        current_stock_node->data->name, 
                        current_stock_node->data->desc);
                /*Removes the stock from list*/
                remove_from_stock_list(system->item_list, previous_stock_node, 
                                        current_stock_node);
                return TRUE;
            }
            /*prints an error if the current stock node is at the end of the
            list*/
            else
            {
                printf("Item with such id does not exist.\n");
            }
        }
    }
    return FALSE;
}

/*Sets teh stock on hand value to default values*/
BOOLEAN reset_stock(struct ppd_system * system)
{
    reset_stock_quantity(system);
    return TRUE;
}

/*Sets the coin count to default values*/
BOOLEAN reset_coins(struct ppd_system * system)
{
    reset_cash_register(system);
    return TRUE;
}

/*Prints all cash register information*/
BOOLEAN display_coins(struct ppd_system * system)
{
    print_cash_register(system);
    return TRUE;
}

/*simple function that returns false*/
/*Used to exit program*/
BOOLEAN abort_program()
{
    return FALSE;   
}