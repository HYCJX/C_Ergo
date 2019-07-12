#ifndef _PLAYER_
#define _PLAYER_

#include <stdlib.h>
#include <stdio.h>
#include "cardCollection.h"

Player *clonePlayer(Player *player);

Player *newPlayer(char variable);

void swapCardsInHand(Player *player, int i1, int i2);

#endif
