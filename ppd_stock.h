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
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "ppd_coin.h"
#include "ppd_shared.h"
#ifndef PPD_STOCK
#define PPD_STOCK

/**
 * @file ppd_stock.h this file defines the data structures required to 
 * manage the stock list. You should add here the function prototypes for
 * managing the list here and implement them in ppd_stock.c
 **/

/**
 * The length of the id string not counting the nul terminator
 **/
#define IDLEN 5

/**
 * The maximum length of a product name not counting the nul terminator
 **/
#define NAMELEN 40

/**
 * The maximum length of a product description not counting the nul
 * terminator.
 **/
#define DESCLEN 255

/**
 * The default coin level to reset the coins to on request
 **/
#define DEFAULT_COIN_COUNT 20

/**
 * The default stock level that all new stock should start at and that 
 * we should reset to on restock
 **/
#define DEFAULT_STOCK_LEVEL 20

/**
 * The number of denominations of currency available in the system 
 **/
#define NUM_DENOMS 8

/*Used in displaying items */
#define MIN_DASHES 30

/*Caps user input for payment to 4 digits*/
#define PAYMENTLEN 4

/*The cap for cents value*/
#define MAX_CENTS_VALUE 95

/* The cap for dollar value*/
#define MAX_DOLLARS_VALUE 99

/* The cap for quantity */
#define MAX_QUANTITY_VALUE 99

/*Cap user input for price when adding products*/
#define PRICELEN 5

/*Highest ID value that can be generated*/
#define LASTID 9999
/**
 * a structure to represent a price. One of the problems with the floating
 * point formats in C like float and double is that they have minor issues
 * of inaccuracy due to rounding. In the case of currency this really is
 * not acceptable so we introduce our own type to keep track of currency.
 **/
struct price
{
    /**
     * the dollar value for some price
     **/
    unsigned dollars, 
             /**
              * the cents value for some price
              **/
             cents;
};

/**
 * data structure to represent a stock item within the system
 **/
struct ppd_stock
{
    /**
     * the unique id for this item
     **/
    char id[IDLEN + 1];
    /**
     * the name of this item
     **/
    char name[NAMELEN + 1];
    /**
     * the description of this item
     **/
    char desc[DESCLEN + 1];
    /**
     * the price of this item
     **/
    struct price price;
    /**
     * how many of this item do we have on hand? 
     **/
    unsigned on_hand;
    /**
     * a pointer to the next node in the list
     **/
};

/**
 * the node that holds the data about an item stored in memory
 **/
struct ppd_node
{
    /* pointer to the data held for the node */
    struct ppd_stock * data;
    /* pointer to the next node in the list */
    struct ppd_node * next;
};

/**
 * the list of products - each link is the list is a @ref ppd_node
 **/
struct ppd_list
{
    /**
     * the beginning of the list
     **/
    struct ppd_node * head;
    /**
     * how many nodes are there in the list?
     **/
    unsigned count;
};

/**
 * this is the header structure for all the datatypes that is 
 * passed around and manipulated
 **/
struct ppd_system
{
    /**
     * the container for all the money manipulated by the system
     **/
    struct coin cash_register[NUM_DENOMS];

    /**
     * the linked list - note that this is a pointer - how does that
     * change what we need to do for initialization of the list?
     **/
    struct ppd_list * item_list;

    /**
     * the name of the coin file - we need this for saving as all menu
     * items only take the one parameter of the ppd_system
     **/
    const char * coin_file_name;
    /**
     * the name of the stock file
     **/
    const char * stock_file_name;

    /* are the coins loaded in from a file ? */
    BOOLEAN coin_from_file;
};

/*Initialize stock list*/
struct ppd_list * init_stock_list();
/*add stock to list*/
BOOLEAN add_to_stock_list(struct ppd_list *, struct ppd_stock *);
/*remove stock from list*/
void remove_from_stock_list(struct ppd_list *, struct ppd_node *, 
                            struct ppd_node*);
/*print stock list*/
void print_stock_list(struct ppd_list *);
/*free memory of stock list*/
struct ppd_list * free_stock_list(struct ppd_list *);
/*resets all quantity to 20*/
void reset_stock_quantity(struct ppd_system *);
/*Generates stock id when adding new stock*/
int generate_stock_id_number(struct ppd_system *);
/*validate and add new stock name to new stock*/
BOOLEAN ask_for_stock_name(struct ppd_stock *);
/*validate and add new stock desc to new stock*/
BOOLEAN ask_for_stock_desc(struct ppd_stock *);
/*Ask and add new stock price to new stock*/
BOOLEAN ask_for_stock_price(struct ppd_stock *);
/*Validates stock price from previous function and tokenize dollars and cents*/
BOOLEAN validate_stock_price_input(struct ppd_stock *, char *);
/*This function checks for | in user input for desc and name*/
BOOLEAN check_for_delimiter(char *);
/*Save stock data to a file*/
BOOLEAN save_stock_to_file(struct ppd_system *);
#endif



