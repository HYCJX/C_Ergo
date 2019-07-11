#ifndef _GAME_RULE_
#define _GAME_RULE_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "dataStructures.h"
#include "IOUtils.h"

GameRule *cloneGameRule(GameRule *source);

GameRule *newGameRule();

void setGameRule(GameRule *rule);

#endif