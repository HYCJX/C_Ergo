#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "card.h"
#include "player.h"
#include "premise.h"
#include "cardCollection.h"
#include "gameRule.h"
#include "IOUtils.h"

bool buildExprFromPremises(GameBoard *board);

bool drawCard(Deck *deck, Player *player, int num);

bool execCommand(GameBoard *board, Player *player, char **tokens, bool *isTerminated);

GameBoard *cloneBoard(GameBoard *source);

GameBoard *newBoard(GameRule *rule, int playerNum);

void dispose(GameBoard *board, Player *player);

void extractScores(GameBoard *board, int *dest);

void finalEval(GameBoard *board);

void freeBoard(GameBoard *board);

void startNewRound(GameBoard *board);

void updateScores(GameBoard *board, const int source[MAX_PLAYERS]);

#endif