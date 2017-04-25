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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{
    struct menu_item menu[NUM_MENU_ITEMS];
    struct ppd_system system;
    BOOLEAN active = TRUE;
    BOOLEAN coins_added;
    /* validate command line arguments */
    /*Tracks the presence of a coin file*/
    if(argc == 3)
    {
        coins_added = TRUE;
    }
    /*Indicates the absence of a coin file*/
    else if(argc == 2)
    {
        coins_added = FALSE;
    }
    /*Indicates the absence of both stock and coin file*/
    else if(argc < 2)
    {
        printf("Please pass in at least the stock data file.\n\n");
        return EXIT_FAILURE;
    }
    /*Indicates more than two data files being presented*/
    else
    {
        printf("This program only supports a maximum of two data files.\n\n");
        return EXIT_FAILURE;
    }
    /*Tries to initialize system*/
    if(system_init(&system))
    {
        /*Checks if the coin file was presented*/
        if(coins_added)
        {
            /*sets the system's coin file tracker to TRUE*/
            system.coin_from_file = TRUE;
            /*Trys to load the coin file*/
            /*free memory and exit the program if it fails*/
            if(!load_coins(&system, argv[2]))
            {
                printf("Failed to Load Cash Register...\n");
                printf("Exiting Program...\n");
                free_stock_list(system.item_list);
                return EXIT_FAILURE;
            }
        }
        /*Trys to load the stock file*/
        /*Continue if successful*/
        if(load_stock(&system, argv[1]))
        {
            /*init menu*/
            init_menu(menu);
            while(active)
            {
                /*Prints the main menu*/
                print_menu(menu);
                /* loop, asking for options from the menu */
                /* run each option selected */
                /* until the user quits */
                active = get_menu_choice(menu)(&system);
                if(active == FALSE)
                {
                    /* make sure you always free all memory and close all files 
                    * before you exit the program
                    */
                    printf("Exiting Program...\n");
                    free_stock_list(system.item_list);
                    return EXIT_SUCCESS;
                }
            }
        }
        /*Free memory and exit the program if stock file fails to load*/
        else
        {
            printf("Failed to Load Stock...\n");
            printf("Exiting Program...\n");
            free_stock_list(system.item_list);
            return EXIT_FAILURE;
        }
    }
    /*Free memory and exit the program if system fails to initialize*/
    else
    {
        printf("System Initiation Failed...\n");
        printf("Exiting Program...\n");
        free_stock_list(system.item_list);
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}
