#ifndef _Game_Board_Utils_
#define _Game_Board_Utils_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "dataStructures.h"
#include "premise.h"

bool execDirectPut(GameBoard *board, Player *player, insOpcode opcode, int premiseId, int index);

bool execDiscard(GameBoard *board, Player *player, insOpcode opcode);

bool execFallacy(GameBoard *board, Player *playerFrom, Player *playerTo);

bool execImmediate(GameBoard *board, Player *player, insOpcode opcode, bool *isTerminated);

bool execRevolution(GameBoard *board, Player *player, int premiseId1, int index1, int premiseId2, int index2);

bool execTabulaRasa(GameBoard *board, Player *player, int premiseId, int index);

void disposeCard(GameBoard *board, Card *card);

#endif