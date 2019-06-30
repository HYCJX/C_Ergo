#include "gameRule.h"

#define FAL_JUST_INDEX (!1)

const int vanillaKey[] = {4, 6, 4, 4, 4, 0, 8, 1, 1, 1, 1, 3, 3};

//Helper Functinos
static void doubleDecker(GameRule *rule)
{
    for (int i = 0; i < NUM_OF_CARD_TYPE; i++) {
        rule -> key[i] = rule -> key[i] * 2;
    }
    printf("Rule: Double decker.\n");
}

static void halfDecker(GameRule *rule)
{
    for (int i = 0; i < NUM_OF_CARD_TYPE; i++) {
        rule -> key[i] = (rule -> key[i] + 1) / 2;
    }
    printf("Rule: Half decker.\n");
}

static void setVanilla(GameRule *rule)
{
    rule -> allowDoubleNeg = false;
    rule -> allowParadoxVictory = false;
    rule -> fallacyPenalty = 3;
    rule -> switchPauseDuration = 3;
    rule -> victoryPoint = 50;
    for (int i = 0; i < NUM_OF_CARD_TYPE; i++) {
        rule -> key[i] = vanillaKey[i];
    }
}

//Functions

//Default game rule
GameRule *newGameRule()
{
    GameRule *rule = (GameRule *) malloc(sizeof(GameRule));
    setVanilla(rule);
    return rule;
}

//WHAT ABOUT switchPauseDuration ??? ???
//Default game rule
GameRule *cloneGameRule(GameRule *source)
{
    GameRule *rule = (GameRule *) malloc(sizeof(GameRule));
    rule -> allowDoubleNeg = source -> allowDoubleNeg;
    rule -> allowParadoxVictory = source -> allowParadoxVictory;
    rule -> fallacyPenalty = source -> fallacyPenalty;
    rule -> victoryPoint = source -> victoryPoint;
    for (int i = 0; i< NUM_OF_CARD_TYPE; i++) {
        rule -> key[i] = source -> key[i];
    }
    return rule;
}

void setGameRule(GameRule *rule);