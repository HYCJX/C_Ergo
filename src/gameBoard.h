#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <stdlib.h>
#include <stdio.h>
#include "dataStructures.h"

bool buildExprFromPremises(GameBoard *board);

bool execCommand(GameBoard *board, Player *player, char **tokens, bool *isTerminated);

GameBoard *newBoard(GameRule *rule, int playerNum);

GameBoard *cloneBoard(GameBoard *source);

void dispose(GameBoard *board, Player *player);

void extractScores(GameBoard *board, int *dest);

void finalEval(GameBoard *board);

void freeBoard(GameBoard *board);

void startNewRound(GameBoard *board);

void updateScores(GameBoard *board, const int source[MAX_PLAYERS]);

#endif