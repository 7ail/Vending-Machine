########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2014 Assignment #2
# Full Name        : EDIT HERE
# Student Number   : EDIT HERE
# Course Code      : EDIT HERE
# Program Code     : EDIT HERE
# Start up code provided by Paul Miller
########################################################################

SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h ppd_shared.h
README=ppd_readme requirement13.txt
MAKEFILE=Makefile

CC = gcc
DEBUG = -g
CFLAGS = -Wall -ansi -pedantic $(DEBUG)
PROG = ppd
OBJS = ppd_main.o ppd_menu.o ppd_options.o ppd_utility.o ppd_stock.o ppd_coin.o


all : $(PROG)

$(PROG): $(OBJS)
	@echo "linking..."
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

ppd_main.o : ppd_main.c ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h
	@echo "compiling ppd_main.o"
	$(CC) $(CFLAGS) -c ppd_main.c

ppd_menu.o : ppd_menu.c ppd_menu.h ppd_options.h ppd_utility.h
	@echo "compiling ppd_menu.o"
	$(CC) $(CFLAGS) -c ppd_menu.c

ppd_options.o : ppd_options.c ppd_options.h ppd_utility.h ppd_stock.h
	@echo "compiling ppd_options.o"
	$(CC) $(CFLAGS) -c ppd_options.c

ppd_utility.o : ppd_utility.c ppd_utility.h
	@echo "compiling ppd_utility.o"
	$(CC) $(CFLAGS) -c ppd_utility.c

ppd_stock.o : ppd_stock.c ppd_stock.h ppd_utility.h
	@echo "compiling ppd_stock.o"
	$(CC) $(CFLAGS) -c ppd_stock.c

ppd_coin.o : ppd_coin.c ppd_coin.h ppd_stock.h ppd_shared.h ppd_utility.h
	@echo "compiling ppd_coin.o"
	$(CC) $(CFLAGS) -c ppd_coin.c

clean:
	@echo "cleaning..."
	rm -f *.o $(PROG)

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
archive:
	zip $(USER)a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE)
