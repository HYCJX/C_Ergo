#ifndef _CARD_COLLECTION_
#define _CARD_COLLECTION_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "card.h"

Card *drawOneCard(Deck *deck);

Deck *cloneDeck(Deck* source);

Deck *initializeDeck(const int key[NUM_OF_CARD_TYPE]);

void addCardtoTail(Deck *deck, Card* card);

void freeDeck(Deck *deck);

#endif