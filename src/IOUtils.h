#ifndef _CERGO_IO_UTILS
#define _CERGO_IO_UTILS

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#include "gameBoard.h"

#define VERSION_NAME "0.2.4"

void clearScreen();

void printFalIfAvaliable(Player *player);

void printScoreBoard(GameBoard *board);

void printPremises(GameBoard *board);

void printBoardtoPlayer(GameBoard *board, Player *player);

char **tokenize(char *line);

char inputSingleChar(const char *reminder);

void pauseBeforeSwitchTurn(GameBoard *board, Player *player, int time);

void easterEgg();

void ruleHelpNotification();



#endif // _CERGO_IO_UTILS
