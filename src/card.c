#include "card.h"

Card *newVarCard(const char c)
{
    Card *card = malloc(sizeof(Card));
    card -> type = VAR;
    card -> CardAs.varName = c;
    card -> isDisposed = false;
    return card;
}