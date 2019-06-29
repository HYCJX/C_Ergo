#ifndef _PLAYER_
#define _PLAYER_

#include <stdlib.h>
#include <stdio.h>
#include "cardCollection.h"

bool drawCard(Deck *deck, Player *player, int num);

int findCardInHand(Player *player, int i1, int i2);

Player *newPlayer(char variable);

Player *clonePlayer(Player *player);

void swapCardsInHand(Player *player, int i1, int i2);

#endif