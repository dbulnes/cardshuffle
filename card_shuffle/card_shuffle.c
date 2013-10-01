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
    deckOnTable->bottomCard = NULL;
    deckOnTable->topCard = NULL;
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
    //deckInHand is now empty. 
    deckInHand->bottomCard = NULL;
    deckInHand->topCard = NULL;
//    //The table deck's top card is now the hand deck's top card.
//    deckOnTable->topCard = deckInHand->topCard;
//    //Assign the bottom table card to the top table card initially. They are the same.
//    deckOnTable->bottomCard = deckOnTable->topCard;
//    //The nextCard pointer of the table deck's top card needs to point to the bottom card, however.
//    deckOnTable->topCard->nextCard = deckOnTable->bottomCard;
//    // 
//    deckInHand->topCard = deckInHand->topCard->nextCard;
//    //deckOnTable->topCard->nextCard = NULL;
//    
//    while (deckInHand->topCard->nextCard) {
//        //Move the top card in the hand deck to the bottom.
//        deckInHand->bottomCard->nextCard = deckInHand->topCard;
//        deckInHand->bottomCard = deckInHand->topCard;
//        deckInHand->topCard = deckInHand->topCard->nextCard;
//        deckInHand->bottomCard->nextCard = NULL;
//        
//        card *previousTopCardOnTable = deckOnTable->topCard;
//        deckOnTable->topCard = deckInHand->topCard;
//        deckInHand->topCard = deckInHand->topCard->nextCard;
//        deckOnTable->topCard->nextCard = previousTopCardOnTable;
//    }
}

void putDeckBackInHand(deck *deckInHand, deck *deckOnTable)
{
    card *temp = deckOnTable->topCard;
    //deckInHand->topCard->nextCard = deckOnTable->topCard->nextCard;
    deckInHand->topCard = temp;
    while (temp->nextCard != deckOnTable->bottomCard) {
        temp = temp->nextCard;
    }
    deckInHand->bottomCard = temp->nextCard;
}

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
        deckOnTable = NULL;
        shuffleCount++;
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
        printf("Deck size: %d\nNumber of shuffles to get back to original deck order: %lld\n", deckSize, roundsToOriginalOrder);
        return 0;
    }
    else {
        printf("Specify the number of cards in the deck.\n");
        return ERROR_RETURN;
    }
}
