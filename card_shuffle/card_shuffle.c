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
        //A deck goes from bottom to top lowest to highest values. Start at 0 for the value, since value itself is
        //irrelevent as long as the initial order of values is sequentially established. 
        newDeck->bottomCard = initializeCard(0, NULL);
        newDeck->topCard = newDeck->bottomCard;
        //A little unconvential, counting backwards. Makes sense with bottom card being highest value. 
        //for (int cardCount = deckSize; cardCount > 0; cardCount--) {
        for (int cardValue = 1; cardValue < deckSize; cardValue++) {
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

int findNumberOfShufflesToOriginalOrder(int deckSize)
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
    
    int shuffleCount;
    for(shuffleCount = 0; !isDeckInOriginalOrder(deckInHand); shuffleCount++){
        shuffle(&deckInHand, &deckOnTable);
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
        
        int roundsToOriginalOrder = findNumberOfShufflesToOriginalOrder(deckSize);
        if (roundsToOriginalOrder == ERROR_RETURN) {
            printf("Error finding the number of rounds for deck size: %d/n", deckSize);
            return ERROR_RETURN;
        }
        printf("Deck size: %d\nNumber of shuffles to get back to original deck order: %d\n", deckSize, roundsToOriginalOrder);
        return 0;
    }
    else {
        printf("Specify the number of cards in the deck.\n");
        return ERROR_RETURN;
    }
}
