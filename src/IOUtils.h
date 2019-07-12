#ifndef _IO_UTILS_
#define _IO_UTILS_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "dataStructures.h"
#include "card.h"
#include "premise.h"

#define VERSION_NAME "0.2.4"

char **tokenize(char *line);

char inputSingleChar(const char *reminder);

void clearScreen();

void countDown();

void pauseBeforeSwitchTurn(GameBoard *board, Player *player, int time);

void printBoardtoPlayer(GameBoard *board, Player *player);

void printFalIfAvaliable(Player *player);

void printPremises(GameBoard *board);

void printScoreBoard(GameBoard *board);

/*-----Special Rules-----*/

void easterEgg();

void ruleHelpNotification();

#endif