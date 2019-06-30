#ifndef _CERGO_DATA_STRUCTURES_
#define _CERGO_DATA_STRUCTURES_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define DEBUG (true)
#define MAX_PREMISE__LENGTH (64)
#define NUM_OF_CARD_TYPE (13)
#define MAX_PLAYERS (4)
#define MAX_PREMISES (4)
#define FIND_FAIL (-1)

enum Operator {
    NO_OPERATOR, NOT, AND, OR, IMPLIES, IFF
};
typedef enum Operator Operator;

enum CardType {
    VAR, OP, PAREN, WILD_VAR, WILD_OP, TABULA_RASA, REVOLUTION, FALLACY, JUSTIFICATION, ERGO, NO_TYPE
};
typedef enum CardType CardType;

struct Card {
    CardType type;
    union {
        char varName;
        Operator op;
        bool isLeft;
    } CardAs;
    bool isDisposed;
};
typedef struct Card Card;

struct DeckNode {
    Card *card;
    struct DeckNode *next;
};
typedef struct DeckNode DeckNode;

struct Deck {
    DeckNode *head;
    DeckNode *tail;
    int size;
};
typedef struct Deck Deck;

struct BoolExpr {
    char variable;
    Operator op;
    struct BoolExpr *leftExpr;
    struct BoolExpr *rightExpr;
};
typedef struct BoolExpr BoolExpr;

struct Premise {
    Card **card;
    int size;
};
typedef struct Premise Premise;

struct Player {
    int score;
    int fallacyCtr;
    char chosenVar;
    bool isJustified;
    Card *hand[7];
    int handCtr;
};
typedef struct Player Player;

struct GameRule {
    bool allowDoubleNeg;
    bool allowParadoxVictory;
    int fallacyPenalty;
    int switchPauseDuration;
    int victoryPoint;
    int key[NUM_OF_CARD_TYPE];
};
typedef struct GameRule GameRule;

struct GameBoard {
    int numOfPlayers;
    int discardIndex;
    Premise *premise[MAX_PREMISES];
    BoolExpr *expr[MAX_PREMISES];
    Player *player[4];
    Deck *deck;
    GameRule *rule;
    Card *discardPile[256];    
};
typedef struct GameBoard GameBoard;

enum insOpcode {
    //direct put
    OP_A, OP_B, OP_C, OP_D,
    OP_WA, OP_WB, OP_WC, OP_WD,
    OP_NOT, OP_AND, OP_OR, OP_IMP, OP_IFF,
    OP_WNOT, OP_WAND, OP_WOR, OP_WIMP,
    OP_PAL, OP_PAR,
    //special
    OP_TR, OP_REV, OP_ERGO,
    OP_FAL, OP_JUST, OP_WJUST,
    OP_WVJUST, OP_WOPJUST,
    //utility
    OP_DISCARD,
    OP_HELP,
    OP_SORTHAND,
    OP_CLS,
    OP_END
};
typedef enum insOpcode insOpcode;

#endif