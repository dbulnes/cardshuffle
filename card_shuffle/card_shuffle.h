//
//  card_shuffle.h
//  Card Shuffle
//
//  Created by David on 9/29/13.
//  Copyright (c) 2013 David Bulnes, david@davidbulnes.com. All rights reserved.
//

#ifndef _card_shuffle_h
#define _card_shuffle_h

#include <stdint.h>
#include <limits.h>

/* 
 Assignment:
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
*/

/*
 Let's define the size of a card's value.
 It can be safely assumed that this program will be run on a 32 bit or 64 bit desktop processor,
 so the 16 bit int should be fine. Unsigned int- no need for negative card values. 
 
 ***If we were to run this on a microcontroller or other processor with 8 bit architecture,
 simply change the below typedef to a type of uint8_t or similar, which will also affect
 the maximum solvable deck size.***
*/
typedef int cardValue;
//bitshift 1 by the number of bits per int.
#define MAX_DECK_SIZE INT_MAX

#define ERROR_RETURN -1

/*
 Per the instructions, in lieu of using an array, a singly linked list should suffice for
 our card data structure.
*/

/** @struct card
    @brief A representation for each card in our deck. Models a forward-pointing linked list. 
    @var value
    Holds the value of the card, which for our purposes doesn't mean the face value of the card. Rather, it
    represents the card's index relative to the order of the original unshuffled deck.
    @var nextCard
    Points to the next card. Should only be NULL, after initialization, for the top card of a deck and/or
    decks with only one card. 
*/
typedef struct card {
    cardValue value;
    struct card *nextCard;
}card;

/** @struct deck
    @brief Keeps pointers to the top and bottom cards of a collection of linked cards, a deck.
    @var bottomCard
    Points to the bottom card on the deck. When a deck is initialized, this card has the highest value.
    @var topCard
    Points to the top card on the deck. When a deck is initialized, this card has the lowest value.
 */
typedef struct deck {
    struct card *bottomCard;
    struct card *topCard;
}deck;

#endif
