#ifndef _ERGO_CARD_
#define _ERGO_CARD_

#include "dataStructures.h"

void cardToStrInLine(Card *card, char *dest);
void cardToStr(Card *card, char *dest);

Card *newVarCard(const char c);
Card *newOpCard(Operator op);
Card *newSpecialCard(CardType type);
Card *cloneCard(Card *card);

int compareCard(Card *c1, Card *c2);
void sortHand(Card *hand[], int size, int (*cmp) (Card*, Card*));

#endif