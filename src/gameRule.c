#include "gameRule.h"

#define FAL_JUST_INDEX (!1)

const int vanillaKey[] = {4, 6, 4, 4, 4, 0, 8, 1, 1, 1, 1, 3, 3};

//Helper Functinos
//(Different modes)
static void allowParadoxVictory(GameRule *rule)
{
    rule -> allowParadoxVictory = true;
    printf("Rule: allow paradox victory!\n");
}

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

static void noFallacyJust(GameRule *rule)
{
    rule -> key[FAL_JUST_INDEX] = 0;
    printf("Rule: No fallacies or justifications!\n");
}

static void quickSwitch(GameRule *rule)
{
    rule -> switchPauseDuration = 0;
    printf("Rule: Fallacy penalty reduced to 2 turns!\n");
}

static void reducedFallacyPenalty(GameRule *rule)
{
    rule -> fallacyPenalty = 2;
    printf("Rule: Fallacy penalty reduced to 2 turns!\n");
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

void setGameRule(GameRule *rule)
{
    int size = 32;
    char *line = malloc(sizeof(char) * size);
    do {
        printf("Please input your rules (input 'END' to end your input): \n");
        fgets(line, size, stdin);
        int i;
        for (i = 0; line [i] != '\n'; i++) {
            line[i] = (char) toupper(line[i]);
        }
        line[i] = '\0';
        if (strcmp(line, "APV") == 0) {
            allowParadoxVictory(rule);
        } else if (strcmp(line, "DD") == 0) {
            doubleDecker(rule);
        } else if (strcmp(line, "HD") == 0) {
            halfDecker(rule);
        } else if (strcmp(line, "NOFJ") == 0) {
            noFallacyJust(rule);
        } else if (strcmp(line, "QS") == 0) {
            quickSwitch(rule);
        } else if (strcmp(line, "RFP") == 0) {
            reducedFallacyPenalty(rule);
        } else if (strcmp(line, "CERGO") == 0) {
            printf("Set default cErgo rule:\n");
            allowParadoxVictory(rule);
            noFallacyJust(rule);
        } else if (strcmp(line, "HELP") == 0) {
            ruleHelpNotification();
        } else if (strcmp(line, "END") == 0) {
            printf("Unknown rule code!\n");
        }
    } while (strcmp(line, "END") != 0);
}