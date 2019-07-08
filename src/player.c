#include "player.h"

//Apply the effects of drawing a Card on a Player and the Deck.
//Return true if the Deck isn't empty.
bool drawCard(Deck *deck, Player *player, int num)
{
    for (int i = 0; i < num; i++) {
        Card *card = drawOneCard(deck);
        player->hand[player->handCtr] = card;
        player->handCtr++;
    }
    return deck->size > 0;
}

//Return a new deeply cloned Player of @param player.
Player *clonePlayer(Player *player)
{
    if (player == NULL) return NULL;
    Player *p = newPlayer(player->chosenVar);
    p->isJustified = player->isJustified;
    p->fallacyCtr = player->fallacyCtr;
    p->handCtr = player->handCtr;
    p->score = player->score;
    for (int i = 0; i < 7; i++) {
        p->hand[i] = cloneCard(player->hand[i]);
    }
    return p;
}

//A constructor that constructs a new Player.
Player *newPlayer(char variable)
{
    Player *p = (Player *) malloc(sizeof(Player));
    p->isJustified = false;
    p->chosenVar = variable;
    p->fallacyCtr = 0;
    p->handCtr = 0;
    p->score = 0;
    for (int i = 0; i < 7; i++) {
        p->hand[i] = NULL;
    }
    return p;
}

//Swap two Cards at positions @param i1 and @param i2 of Player @param player.
void swapCardsInHand(Player *player, int i1, int i2)
{
    Card *tmp = player->hand[i1];
    player->hand[i1] = player->hand[i2];
    player->hand[i2] = tmp;
}