#ifndef _GAME_RULE_
#define _GAME_RULE_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "dataStructures.h"

GameRule *newGameRule();

GameRule *cloneGameRule(GameRule *source);

void setGameRule(GameRule *rule);

#endif