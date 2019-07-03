#include "player.h"

Player *newPlayer(char variable) {
    Player *p = (Player *) malloc(sizeof(Player));
    p->chosenVar = variable;
    p->fallacyCtr = 0;
    for (int i = 0; i < 7; i++) {
        p->hand[i] = NULL;
    }
    p->isJustified = false;
    p->score = 0;
    p->handCtr = 0;
    return p;
}

Player *clonePlayer(Player *player) {
    if (player == NULL) return NULL;
    Player *p = newPlayer(player->chosenVar);
//    p->chosenVar = player->chosenVar;
    p->fallacyCtr = player->fallacyCtr;
    for (int i = 0; i < 7; i++) {
        p->hand[i] = cloneCard(player->hand[i]);
    }
    p->isJustified = player->isJustified;
    p->score = player->score;
    p->handCtr = player->handCtr;
    return p;
}


void swapCardsInHand(Player *player, int i1, int i2) {
    Card *tmp = player->hand[i1];
    player->hand[i1] = player->hand[i2];
    player->hand[i2] = tmp;
}

bool drawCard(Deck *deck, Player *player, int num) {
    for (int i = 0; i < num; i++) {
        Card *card = drawOneCard(deck);
        player->hand[player->handCtr] = card;
        player->handCtr++;
    }
    return deck->size > 0;
}