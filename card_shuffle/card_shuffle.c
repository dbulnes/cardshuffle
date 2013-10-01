//
//  card_shuffle.c
//  Card Shuffle
//
//  Created by David on 9/29/13.
//  Copyright (c) 2013 David Bulnes. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "card_shuffle.h"

/* TODO: 
    -Write shuffle algorithm
    -comments everywhere
    -question 2 write up.
 */

card* initializeCard(int value, card *nextCard)
{
    //No need to calloc or mem-set as we are assigning new values to memory immediately
    card *newCard = malloc(sizeof(struct card));
    newCard->value = value;
    newCard->nextCard = nextCard;
    return newCard;
}

deck* initializeDeck(int deckSize)
{
    //Calloc so that all card pointers are zeroed. They will be assigned later.
    deck *newDeck = calloc(1, sizeof(struct deck));
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

card *unlinkTopCardFromDeck(deck *deck)
{
    //If a next card exists after the top card, assign it, otherwise the top card and bottom card are equal.
    card *newTopCard = deck->topCard->nextCard ? deck->topCard->nextCard : deck->bottomCard;
    deck->topCard->nextCard = NULL;
    card *unlinkedTopCard = deck->topCard;
    deck->topCard = newTopCard;
    return unlinkedTopCard;
}

void putCardOnTopOfDeck(card *card, deck *deck)
{
    card->nextCard = deck->topCard;
    deck->topCard = card;
    return;
}


void putCardOnBottomOfDeck(card *card, deck *deck)
{
    deck->bottomCard->nextCard = card;
    card->nextCard = NULL;
    deck->bottomCard = card;
    return;
}

/** @brief Empties a deck by NULLing the card pointers of a given deck.
 *
 *  @param deck A deck pointer that is to have it's bottom and top cards NULLed
 *  @return void
 */
void emptyDeck(deck *deck)
{
    deck->bottomCard = NULL;
    deck->topCard = NULL;
}

/** @brief Performs one round of the shuffle algorithm on a deck, divided into two sub-decks.
*  Algorithm: 
*       "1. Take the top card off the deck and set it on the table
*        2. Take the next card off the top and put it on the bottom of the deck
*        in your hand.
*        3. Continue steps 1 and 2 until all cards are on the table.  This is a
*        round."
*  
*  @param deckInHand A dereferenced pointer to the deck in hand.
*  @param deckOnTable A dereferenced pointer to the deck on the table.
*  @return void
*/
void shuffleDeckOneRound(deck *deckInHand, deck *deckOnTable)
{
    while (deckInHand->topCard != deckInHand->bottomCard) {
        card *cardToPutOnTable = unlinkTopCardFromDeck(deckInHand);
        putCardOnTopOfDeck(cardToPutOnTable, deckOnTable);
        //Check if the bottom table card is empty, i.e. this is the first iteration of the loop.
        //If so, the bottom card and top card on the table are the same
        if (!deckOnTable->bottomCard)
            deckOnTable->bottomCard = deckOnTable->topCard;
        
        card *cardToPutOnBottomOfHand = unlinkTopCardFromDeck(deckInHand);
        putCardOnBottomOfDeck(cardToPutOnBottomOfHand, deckInHand);
    }
    //Take the last card in the hand deck and put it on top of the table deck.
    card *cardToPutOnTable = unlinkTopCardFromDeck(deckInHand);
    putCardOnTopOfDeck(cardToPutOnTable, deckOnTable);
    //Make sure the deck in hand is empty.
    emptyDeck(deckInHand);
}

/** @brief Puts the deck on the table back in hand. Uses a temporary card pointer to assign the 
 *  hand deck's top and bottom cards.
 *
 *  @param deckInHand A dereferenced pointer to the deck in hand.
 *  @param deckOnTable A dereferenced pointer to the deck on the table.
 *  @return void
 */
void putDeckBackInHand(deck *deckInHand, deck *deckOnTable)
{
    card *tempCard = deckOnTable->topCard;
    deckInHand->topCard = tempCard;
    while (tempCard->nextCard != deckOnTable->bottomCard) {
        tempCard = tempCard->nextCard;
    }
    deckInHand->bottomCard = tempCard->nextCard;
    //Now that the deck in hand has the cards of the table deck, empty the table deck's pointers.
    emptyDeck(deckOnTable);
}

int isDeckInOriginalOrder(deck *deck)
{
    struct card *cardIterator = deck->topCard;
    for (int cardCount = 1; cardIterator; cardCount++) {
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
    //Check for the case of 0,1,2,3 deck size. These values only need [deckSize] number of shuffles.
    //No need for further computation.
    if (deckSize <= 3) {
        return deckSize;
    }
    
    //Instead of making shuffleDeckOneRound() do the work of mallocing and freeing a temporary table deck
    //[deckSize] number of times, let's initialize one here and free it when done with all shuffle rounds. 
    deck *deckOnTable = initializeDeck(0);
    //Create the real deck we will be examining to determine sorted order. 
    deck *deckInHand = initializeDeck(deckSize);
    
    if (deckOnTable == NULL || deckInHand == NULL){
        printf("Error initializing decks.");
        return ERROR_RETURN;
    }
    
    uint64_t shuffleCount = 0;
    do {
        shuffleDeckOneRound(deckInHand, deckOnTable);
        //deckInHand = deckOnTable;
        putDeckBackInHand(deckInHand, deckOnTable);
        shuffleCount++;
        //printf("%lld\n", shuffleCount);
    } while (!isDeckInOriginalOrder(deckInHand));
    
    //free all memory- or talk about how id do it if there was more to be done after
    return shuffleCount;
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
        
        printf("Deck size: %d ::: Number of shuffles to get back to original deck order: %lld\n", deckSize, roundsToOriginalOrder);
        return 0;
    }
    else {
        printf("Specify the number of cards in the deck.\n");
        return ERROR_RETURN;
    }
}
