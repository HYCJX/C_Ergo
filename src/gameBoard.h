#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "gameBoardUtils.h"
#include "gameRule.h"
#include "player.h"

bool buildExprFromPremises(GameBoard *board);

bool drawCard(Deck *deck, Player *player, int num);

bool execCommand(GameBoard *board, Player *player, char **tokens, bool *isTerminated);

GameBoard *cloneBoard(GameBoard *source);

GameBoard *newBoard(GameRule *rule, int playerNum);

void dispose(GameBoard *board, Player *player);

void extractScores(GameBoard *board, int *dest);

void finalEval(GameBoard *board);

void freeBoard(GameBoard *board);

void updateScores(GameBoard *board, const int source[MAX_PLAYERS]);

#endif