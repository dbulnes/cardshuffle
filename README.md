card_shuffle README

Created by David Bulnes

***Instructions:***

--Compiling program and creating an executable:*
	>make 
	OR
	>make card_shuffle

--Single argument input: The number of cards in the deck to run the
shuffle algorithm on.

--Example execution, input to, and output of card_shuffle program:
    >./card_shuffle 11
    Deck size: 11
    Shuffles needed to return to original deck order: 15


 ***Assignment:***
 You are given a deck containing N cards.  While holding the deck:
 
 1. Take the top card off the deck and set it on the table
 2. Take the next card off the top and put it on the bottom of the deck
 in your hand.
 3. Continue steps 1 and 2 until all cards are on the table.  This is a
 round.
 4. Pick up the deck from the table and repeat steps 1-3 until the deck
 is in the original order.
 
 Write a program to determine how many rounds it will take to put a
 deck back into the original order.  This will involve creating a data
 structure to represent the order of the cards. Do not use an array.
 This program should be written in C only. It should take a number of
 cards in the deck as a command line argument and write the result to
 stdout.  Please ensure the program compiles and runs correctly (no
 pseudo-code).  This isn't a trick question; it should be fairly
 straightforward.