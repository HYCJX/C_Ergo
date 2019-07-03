#ifndef _CARD_COLLECTION_
#define _CARD_COLLECTION_

#include "card.h"
#include "dataStructures.h"

void addCardtoTail(Deck *deck, Card* card);

Deck *initializeDeck(const int key[NUM_OF_CARD_TYPE]);

Card *drawOneCard(Deck *deck);

Deck *cloneDeck(Deck* source);

void freeDeck(Deck *deck);

#endif