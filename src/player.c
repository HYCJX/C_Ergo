#include "player.h"

//WHERE IS findCardInHand ??? ???

static const int INITIAL_CARD_NUM = 7;

bool drawCard(Deck *deck, Player *player, int num)
{
    for (int i = 0; i < num; i++) {
        Card *card = drawOneCard(deck);
        player -> hand[player -> handCtr] = card;
        player -> handCtr++;
    }
    return deck -> size > 0;
}

Player *newPlayer(char variable)
{
    Player *player = (Player *) malloc(sizeof(Player));
    player -> chosenVar = variable;
    player -> fallacyCtr = 0;
    for (int i = 0; i < INITIAL_CARD_NUM; i++) {
        player -> hand[i] = NULL;
    }
    player -> isJustified = false;
    player -> score = 0;
    player -> handCtr = 0;
    return player;
}

Player *clonePlayer(Player *player)
{
    if (player == NULL) return NULL;
    Player *clonedPlayer = newPlayer(player -> chosenVar);
    clonedPlayer -> fallacyCtr = player -> fallacyCtr;
    for (int i = 0; i < INITIAL_CARD_NUM; i++) {
        clonedPlayer -> hand[i] = cloneCard(player -> hand[i]);
    }
    clonedPlayer -> isJustified = player -> isJustified;
    clonedPlayer -> score = player -> score;
    clonedPlayer -> handCtr = player -> handCtr;
    return clonedPlayer;
}

void swapCardsInHand(Player *player, int i1, int i2)
{
    Card *tmp = player -> hand[i1];
    player -> hand[i1] = player -> hand[i2];
    player -> hand[i2] = tmp;
}