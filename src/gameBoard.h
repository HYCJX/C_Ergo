#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <stdint.h>
#include <stdbool.h>
#include "card.h"
#include "player.h"
#include "premise.h"
#include "cardCollection.h"
#include "gameRule.h"
#include "IOUtils.h"

GameBoard* newBoard(GameRule *rule, int playerNum);

GameBoard* cloneBoard(GameBoard *source);

bool drawCard(Deck *deck, Player *player, int num);

//bool isGameTerminated(GameBoard *board);

bool buildExprFromPremises(GameBoard *board);

bool execCommand(GameBoard *board, Player *player, char **tokens, bool *isTerminated);

void dispose(GameBoard *board, Player *player);

void finalEval(GameBoard *board);

void startNewRound(GameBoard *board);

void updateScores(GameBoard *board, const int source[MAX_PLAYERS]);

void extractScores(GameBoard *board, int *dest);

void freeBoard(GameBoard *board);

#endif