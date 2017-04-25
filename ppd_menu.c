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

#include "ppd_menu.h"
#include "ppd_options.h" /* added on my own */
#include "ppd_utility.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu(struct menu_item * menu)
{
	/*create and temporary array of struct menu_item*/
	struct menu_item x[NUM_MENU_ITEMS] =
	{
		{"1.Display Items", display_items},
		{"2.Purchase Items", purchase_item},
		{"3.Save and Exit", save_system},
		{"4.Add Item", add_item},
		{"5.Remove Item", remove_item},
		{"6.Display Coins", display_coins},
		{"7.Reset Stock", reset_stock},
		{"8.Reset Coins", reset_coins},
		{"9.Abort Program", abort_program}
	};
	/*Copy it to the main menu_item array*/
	memcpy(menu, x, sizeof(x));
}

/**
 * @return a menu_function that defines how to perform the user's
 * selection
 **/
menu_function get_menu_choice(struct menu_item * menu)
{
    char selection[3];
    char *remaining_ptr = NULL;
    int menu_selection = 0;
    while(TRUE)
    {
    	/* get the user's selection for the main menu*/
	    if(fgets(selection, sizeof(selection), stdin) == NULL)
	    {
	        printf("Invalid input.\n");
	        printf("Please input numbers between 1 to %d.\n", NUM_MENU_ITEMS);
	        continue;
	    }
	    /*Clears the buffer if the input is too large*/
	    if(selection[strlen(selection) - 1] != NEWLINE)
		{
			read_rest_of_line();
			/*Prompts for a valid input*/
	        printf("Input too large - try again. \n");
	        printf("Please input numbers between 1 to %d.\n", NUM_MENU_ITEMS);
	        continue;
		}
	    /*Cast user input into an int */
		menu_selection = (int) strtol(selection, &remaining_ptr, 10);
		/*Checks if the input is within 1-9*/
		if(menu_selection <= 0 || menu_selection > NUM_MENU_ITEMS)
		{
			printf("Invalid input.\n");
	        printf("Please input numbers between 1 to %d.\n", NUM_MENU_ITEMS);
	        continue;
		}
		else
		{
			/*Compensate user input for array indexing*/
			menu_selection -= 1;
			/*Returns a menu function pointer*/
			return menu[menu_selection].function;
		}
    }
    return NULL;
}

/*Function used to print main menu*/
void print_menu(struct menu_item * menu)
{
	int count = 0;
	int menu_func_count = 0;
	/*Depending on the value of count different predetermined header positions
	will print*/
	for(count = 0; count < NUM_MENU_LINES; count++)
	{
		if(count == 0)
		{
			printf("Main Menu:\n");
		}
		else if(count == 4)
		{
			printf("Administrator-Only Menu:\n");
		}
		else if(count == NUM_MENU_LINES - 1)
		{
			printf("Select your option (1-%d):", NUM_MENU_ITEMS);
		}
		/*Prints the strings saved in each struct menu_item*/
		else
		{
			printf("%s\n", menu[menu_func_count].name);
			menu_func_count++;
		}
	}
}
