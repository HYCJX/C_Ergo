#ifndef _CARD_
#define _CARD_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "dataStructures.h"

int compareCard(Card *c1, Card *c2);

Card *cloneCard(Card *card);

Card *newOpCard(Operator op);

Card *newSpecialCard(CardType type);

Card *newVarCard(const char c);

void cardToStrInLine(Card *card, char *dest);

void cardToStr(Card *card, char *dest);

void sortHand(Card *hand[], int size, int (*cmp) (Card*, Card*));

#endif