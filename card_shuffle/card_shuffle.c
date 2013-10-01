//
//  card_shuffle.c
//  Card Shuffle
//
//  Created by David on 9/29/13.
//  Copyright (c) 2013 David Bulnes. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "card_shuffle.h"

/* TODO: 
    -Write shuffle algorithm
    -comments everywhere
    -question 2 write up.
 */


/** @brief Performs one round of the shuffle algorithm on a deck, divided into two sub-decks.
*  Algorithm: 
*       "1. Take the top card off the deck and set it on the table
*       2. Take the next card off the top and put it on the bottom of the deck
*       in your hand.
*       3. Continue steps 1 and 2 until all cards are on the table.  This is a
*       round."
*
*  @param deckInHand The row in which to display the character.
*  @param deckOnTable The column in which to display the character.
*  @return void
*/
void shuffleDeckOneRound(deck *deckInHand, deck *deckOnTable)
{
    
}

card* initializeCard(int value, card *nextCard)
{
    card *newCard = malloc(sizeof(struct card));
    newCard->value = value;
    newCard->nextCard = nextCard;
    return newCard;
}

deck* initializeDeck(int deckSize)
{
    deck *newDeck = malloc(sizeof(struct deck));
    //Check for non zero deck size.
    if(deckSize){
        //A deck's initial pointers - top card: lowest value, bottom card: highest value 
        newDeck->bottomCard = initializeCard(deckSize, NULL);
        newDeck->topCard = newDeck->bottomCard;
        //A little unconvential, counting backwards. Makes sense with bottom card being highest value. 
        for (int cardValue = deckSize-1; cardValue > 0; cardValue--) {
        //for (int cardValue = 1; cardValue < deckSize; cardValue++) {
            card *newCard = initializeCard(cardValue, newDeck->topCard);
            newDeck->topCard = newCard;
        }
    }
    return newDeck;
}

int isDeckInOriginalOrder(deck *deck)
{
    struct card *cardIterator = deck->topCard;
    for (int cardCount = deck->topCard->value; cardCount > 0; cardCount--) {
        if (cardCount == cardIterator->value)
            cardIterator = cardIterator->nextCard;
        else
            return 0;
    }
    return 1;
}

//maybe long instead of long long? make note of
uint64_t findNumberOfShufflesToOriginalOrder(int deckSize)
{
    //Check for the case of one card in the deck. No need for further computation. 
    if (deckSize <= 3) {
        return deckSize;
    }
    
    deck *deckOnTable = initializeDeck(0);
    deck *deckInHand = initializeDeck(deckSize);
    
    if (deckOnTable == NULL || deckInHand == NULL){
        printf("Error initializing decks.");
        return ERROR_RETURN;
    }
    
    uint64_t shuffleCount;
    for(shuffleCount = 0; !isDeckInOriginalOrder(deckInHand); shuffleCount++){
        //shuffle(&deckInHand, &deckOnTable);
    }
    
    return shuffleCount;
    //free all memory- or talk about how id do it if there was more to be done after
}

int main(int argc, const char * argv[])
{
    //Check for valid input argument, and find number of required rounds for given deck size.
    if (argc > 1) {
        int deckSize;
        if (!sscanf (argv[1], "%d", &deckSize)) {
            printf("Error: Invalid input type, not an integer.\n");
            return ERROR_RETURN;
        }
        if (deckSize < 0) {
            printf("Error: Negative deck size. Input a positive deck size.");
            return ERROR_RETURN;
        }
        if (deckSize > MAX_DECK_SIZE) {
            printf("Error: Deck size too large. Max deck size: %u\n", MAX_DECK_SIZE);
            return ERROR_RETURN;
        }
        
        uint64_t roundsToOriginalOrder = findNumberOfShufflesToOriginalOrder(deckSize);
        if (roundsToOriginalOrder == ERROR_RETURN) {
            printf("Error finding the number of rounds for deck size: %d/n", deckSize);
            return ERROR_RETURN;
        }
        printf("Deck size: %d\nNumber of shuffles to get back to original deck order: %lld\n", deckSize, roundsToOriginalOrder);
        return 0;
    }
    else {
        printf("Specify the number of cards in the deck.\n");
        return ERROR_RETURN;
    }
}
