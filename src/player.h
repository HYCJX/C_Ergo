//
// Created by zy7218 on 14/06/19.
//

#ifndef CERGO_PLAYER_H
#define CERGO_PLAYER_H

#include "cardCollection.h"
#include <stdio.h>

Player *newPlayer(char variable);

Player *clonePlayer(Player *player);

void swapCardsInHand(Player *player, int i1, int i2);

int findCardInHand(Player *player, insOpcode opcode);

bool drawCard(Deck *deck, Player *player, int num);

#endif //CERGO_PLAYER_H
