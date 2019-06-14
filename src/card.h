#ifndef _CARD_
#define _CARD_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "dataStructures.h"

static const char *OP_TO_STR[] = {"NO", "-", "&", "V", "->"};

Card *newVarCard(const char c);

Card *newOpCard(Operator op);

Card *newSpecialCard(CardType type);

Card *cloneCard(Card *card);

void CardTpStrInLine(Card *card, char *dest);

void cardToStr(Card *card, char *dest);

void sortHand(Card *hand[], int size, int (*cmp) (Card*, Card*));

int compareCard(Card *c1, Card *c2);

#endif