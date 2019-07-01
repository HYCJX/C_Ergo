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
#include "premise.h"

char inputSingleChar(const char *reminder);

char **tokenize(char *line);

void clearScreen();

void pauseBeforeSwitchTurn(GameBoard *board, Player *player, int time);

void printBaordtoPlayer(GameBoard *board, Player *player);

void printFalIfAvailable(Player *player);

void printPremises(GameBoard *board);

void printScoreBoard(GameBoard *board);

#endif