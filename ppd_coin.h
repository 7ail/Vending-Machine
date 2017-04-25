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
#include "ppd_shared.h"

#ifndef PPD_COIN
#define PPD_COIN
#define COIN_DELIM ","
 
struct ppd_system;
struct ppd_node;

/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
    FIVE_CENTS = 5, TEN_CENTS = 10, TWENTY_CENTS = 20, FIFTY_CENTS = 50, 
    ONE_DOLLAR = 100, TWO_DOLLARS = 200, FIVE_DOLLARS = 500, TEN_DOLLARS = 1000
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

/*Initialize cash register and set all count to 0*/
void init_cash_register(struct ppd_system *);
/*Prints the content of the cash register*/
void print_cash_register(struct ppd_system *);
/*Sets all the denom counts to 20*/
void reset_cash_register(struct ppd_system *);
/*Save any changes to file*/
BOOLEAN save_coins_to_file(struct ppd_system *);
/*handles the change calculation and generates a string showing the denoms*/
BOOLEAN handle_change(struct coin *, char *, int);
/*validates the payments being made and process the payment*/
BOOLEAN make_payment(struct ppd_system *, struct ppd_node *);
#endif
