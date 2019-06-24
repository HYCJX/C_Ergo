#ifndef _CARD_COLLECTION_
#define _CARD_COLLECTION_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "dataStructures.h"
#include "card.h"

Deck *initializeDeck(const int key[NUM_OF_CARD_TYPE]);

Deck *cloneDeck(Deck *source);

Card *drawOneCard(Deck *deck);

void addCardtoTail(Deck *deck, Card *card);

void freeDeck(Deck *deck);

#endif