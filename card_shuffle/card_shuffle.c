//
//  card_shuffle.c
//  Card Shuffle - Program to compute number of required shuffles to complete the algorithm
//  described in card_shuffle.h
//
//  Created by David on 9/29/13.
//  Copyright (c) 2013 David Bulnes. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "card_shuffle.h"

/** @function initializeCard
 *  @brief Empties a deck by NULLing the card pointers of a given deck.
 *
 *  @param deck A deck pointer that is to have it's bottom and top cards NULLed.
 *  @return void
 */
card *initializeCard(cardValue value, card *nextCard)
{
    //No need to calloc or mem-set as we are assigning new values to memory immediately.
    card *newCard = malloc(sizeof(struct card));
    newCard->value = value;
    newCard->nextCard = nextCard;
    return newCard;
}

/** @function initializeDeck
 *  @brief Intializes a deck of a given size. Recursive- initializes all cards in the new deck as well.
 *
 *  @param deckSize The size of the deck. In other words, the number of cards to place in the deck. 
 *  @return deck *
 */
deck* initializeDeck(deckSize deckSize)
{
    //Calloc so that all card pointers are zeroed. They will be assigned later in this function or elsewhere.
    deck *newDeck = calloc(1, sizeof(struct deck));
    //Check for non zero deck size.
    if(deckSize){
        //A deck's initial pointers - top card: lowest value, bottom card: highest value
        newDeck->bottomCard = initializeCard(deckSize, NULL);
        newDeck->topCard = newDeck->bottomCard;
        //A little less convential, counting backwards. Makes sense with bottom card being highest value.
        //Cards are created from the bottom of the deck upwards. 
        cardValue cardValue;
        for (cardValue = deckSize-1; cardValue > 0; cardValue--) {
            card *newCard = initializeCard(cardValue, newDeck->topCard);
            newDeck->topCard = newCard;
        }
    }
    return newDeck;
}

/** @function emptyDeck
 *  @brief Empties a deck by NULLing the card pointers of a given deck.
 *
 *  @param deck A deck pointer that is to have it's bottom and top cards NULLed.
 *  @return void
 */
void emptyDeck(deck *deck)
{
    deck->bottomCard = NULL;
    deck->topCard = NULL;
}

/** @function freeDeck
 *  @brief Frees a deck by freeing the memory of all its associated cards. Also calls emptyDeck to NULL out 
 *  deck's card pointers and NULLs the deck pointer itself as well. 
 *
 *  @param deck A deck pointer to traverse and free all memory pointed to.
 *  @return void
 */
void freeDeck(deck *deck)
{
    //Let's prevent bad memory access when trying to access or free already freed memory. 
    if(deck->topCard){
        //Iterate and free the top card until the card pointers are the same.
        do{
            card *nextTopCard = deck->topCard->nextCard;
            free(deck->topCard);
            deck->topCard = nextTopCard;
        } while (deck->topCard != deck->bottomCard);
    }
    //NULL all card pointers and the deck pointer.
    emptyDeck(deck);
    deck = NULL;
}

/** @function unlinkTopCardFromDeck
 *  @brief Takes the top card off of the given deck, and reassigns the top pointer of the deck to the next card.
 *
 *  @param deck The deck from which to unlink and return the top card.
 *  @return card returns 
 */
card* unlinkTopCardFromDeck(deck *deck)
{
    //If a next card exists after the top card, assign it, otherwise the top card and bottom card are equal.
    card *newTopCard = deck->topCard->nextCard ? deck->topCard->nextCard : deck->bottomCard;
    deck->topCard->nextCard = NULL;
    card *unlinkedTopCard = deck->topCard;
    deck->topCard = newTopCard;
    return unlinkedTopCard;
}

/** @function putCardOnTopOfDeck
 *  @brief Takes a card, and makes it the top card of a given deck.
 *
 *  @param card The card to put on top of the deck.
 *  @param deck The deck to put the card on top of.
 *  @return void
 */
void putCardOnTopOfDeck(card *card, deck *deck)
{
    card->nextCard = deck->topCard;
    deck->topCard = card;
    return;
}

/** @function putCardOnBottomOfDeck
 *  @brief Takes a card, and makes it the bottom card of a given deck.
 *
 *  @param card The card to put on the bottom of the deck.
 *  @param deck The deck to put the card on the bottom of. 
 *  @return void
 */
void putCardOnBottomOfDeck(card *card, deck *deck)
{
    deck->bottomCard->nextCard = card;
    card->nextCard = NULL;
    deck->bottomCard = card;
    return;
}

/** @function shuffleDeckOneRound
 *  @brief Performs one round of the shuffle algorithm on a deck, divided into two sub-decks.
 *  Algorithm:
 *       "1. Take the top card off the (hand) deck and set it on the table
 *        2. Take the next card off the top (of the hand deck) and put it on the bottom of the deck
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
    //The conditional check here is Step 3 in the brief. If the top and bottom card are the same, there is
    //only one card in the hand deck left. 
    while (deckInHand->topCard != deckInHand->bottomCard) {
        //Step 1: Take the top card from the hand deck, put it on the top of the deck on the table. 
        card *cardToPutOnTable = unlinkTopCardFromDeck(deckInHand);
        putCardOnTopOfDeck(cardToPutOnTable, deckOnTable);
        //Check if the bottom table card is empty, i.e. this is the first iteration of the loop.
        //If so, the bottom card and top card on the table should be the same. Needed once per shuffle round.
        if (!deckOnTable->bottomCard)
            deckOnTable->bottomCard = deckOnTable->topCard;
        //Step 2: Take the top card in the hand deck and put it on the bottom of the hand deck.
        card *cardToPutOnBottomOfHand = unlinkTopCardFromDeck(deckInHand);
        putCardOnBottomOfDeck(cardToPutOnBottomOfHand, deckInHand);
    }
    //Take the last card in the hand deck and put it on top of the table deck.
    card *cardToPutOnTable = unlinkTopCardFromDeck(deckInHand);
    putCardOnTopOfDeck(cardToPutOnTable, deckOnTable);
    //Make sure the deck in hand is empty.
    emptyDeck(deckInHand);
}

/** @function putDeckBackInHand
 *  @brief Puts the deck on the table back in hand. Uses a temporary card pointer to assign the
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

/** @function isDeckInOriginalOrder
 *  @brief Iterates over a deck to determine if it is back in its original order, or in other words,
 *   if the deck is from bottom to top composed of cards decreasing from highest to lowest value.
 *
 *  @param deck The deck to put the card on top of.
 *  @return int 1 or 0 to indicate the deck is or is not in it's original order, respectively. 
 */
int isDeckInOriginalOrder(deck *deck)
{
    struct card *cardIterator = deck->topCard;
    cardValue cardCount;
    for (cardCount = 1; cardIterator; cardCount++) {
        if (cardCount == cardIterator->value)
            cardIterator = cardIterator->nextCard;
        else
            return 0;
    }
    return 1;
}

/** @function findNumberOfShufflesToOriginalOrder
 *  @brief The main driver of the program. Initializes a table and hand deck, and shuffles until
 *   the hand deck is back in the original order while incrementing a counter.
 *
 *  @param deckSize The size of the deck to make the initial hand deck. 
 *  @return roundCount (uint64_t) The number of rounds of shuffles it takes for the deck to be sorted in its
 *  original order.
 */
roundCount findNumberOfShufflesToOriginalOrder(deckSize deckSize)
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
    
    //Perform at least one round of shuffling, and continue doing until the deck is back in it's original order.
    roundCount shuffleCount = 0;
    do {
        shuffleDeckOneRound(deckInHand, deckOnTable);
        putDeckBackInHand(deckInHand, deckOnTable);
        shuffleCount++;
    } while (!isDeckInOriginalOrder(deckInHand));
    
    //Free all allocated memory
    freeDeck(deckInHand);
    freeDeck(deckOnTable);
    return shuffleCount;
}

/** @function main
 *  @brief The main loop- validates input, kick off program, prints out results. 
 *  Example execution, input to, and output of card_shuffle program:
 *   >./card_shuffle 11
 *   Deck size: 11
 *   Shuffles needed to return to original deck order: 15
 *
 *
 *  @param deckSize The size of the deck to make the initial hand deck.
 *  @return roundCount (uint64_t) The number of rounds of shuffles it takes for the deck to be sorted in its
 *  original order.
 */
int main(int argc, const char * argv[])
{
    //Check for valid input argument, and find number of required rounds for given deck size.
    if (argc > 1) {
        deckSize deckSize;
        if (!sscanf (argv[1], "%u", &deckSize)) {
            printf("Error: Invalid input type, not an integer.\n");
            return ERROR_RETURN;
        }
        //Check if the deck size is inputted as negative or overflows a uint to two's complement.
        if ((int)deckSize < 0) {
            printf("Error: Invalid deck size. Input a deck size between 0 and %u.\n", MAX_DECK_SIZE);
            return ERROR_RETURN;
        }
        //Find a solution to the algorithm with the given deck size.
        roundCount roundsToOriginalOrder = findNumberOfShufflesToOriginalOrder(deckSize);
        if (roundsToOriginalOrder == ERROR_RETURN) {
            printf("Error finding the number of rounds for deck size: %d/n", deckSize);
            return ERROR_RETURN;
        }
        
        printf("Deck size: %d\nShuffles needed to return to original deck order: %lld\n", deckSize, roundsToOriginalOrder);
        return 0;
    }
    else {
        printf("Specify the number of cards in the deck.\n");
        return ERROR_RETURN;
    }
}
