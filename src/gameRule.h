//
// Created by zy7218 on 05/06/19.
//

#ifndef CERGO_GAMERULE_H
#define CERGO_GAMERULE_H

#include "dataStructures.h"
#include "IOUtils.h"

GameRule *newGameRule();

GameRule *cloneGameRule(GameRule *source);

void setGameRule(GameRule *rule);

#endif //CERGO_GAMERULE_H
