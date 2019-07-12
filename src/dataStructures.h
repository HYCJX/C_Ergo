#ifndef _CERGO_DATA_STRUCTURES_
#define _CERGO_DATA_STRUCTURES_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define DEBUG (true)
#define FIND_FAIL (-1)
#define MAX_PLAYERS (4)
#define MAX_PREMISES (4)
#define MAX_PREMISE_LENGTH (64)
#define NUM_OF_CARD_TYPE (13)

/*-----Enumerations-----*/

enum CardType {
    VAR, OP, PAREN, WILD_VAR, WILD_OP,
    TABULA_RASA, REVOLUTION, FALLACY, JUSTIFICATION, ERGO,
    NO_TYPE
};
typedef enum CardType CardType;

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

//Operators enumerated in decreasing priority
enum Operator {
    NO_OPERATOR, NOT, AND, OR, IMPLIES, IFF
};
typedef enum Operator Operator;

/*-----Structures-----*/

/*---Basic level---*/

//Logic expressions
struct BoolExpr {
    char variable;
    Operator op;
    struct BoolExpr *leftExpr;
    struct BoolExpr *rightExpr;
};
typedef struct BoolExpr BoolExpr;

//Game cards
struct Card {
    CardType type;
    union {
        bool isLeft;  //For parentheses
        char varName; //For variables
        Operator op;  //For operators
    }CardAs;
    bool isDisposed;
};
typedef struct Card Card;

/*---Intermediate level---*/

//Deck of cards to be drawn
//Linked list that stores Cards
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

//Game players
struct Player {
    bool isJustified;
    char chosenVar;
    int fallacyCtr;
    int handCtr;
    int score;
    Card *hand[7];
};
typedef struct Player Player;

//Unevaluated logical expressions
//Array of Cards
struct Premise {
    Card **card;
    int size;
};
typedef struct Premise Premise;

/*---Highest levels---*/

//Rule status
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

    //Two counters:
    int discardIndex;
    int numOfPlayers;

    //Five objects
    Card *discardPile[256];         //1. Discarded cards
    Deck *deck;                     //2. Undrawn cards
    Player *player[4];              //3. Players
    Premise *premise[MAX_PREMISES]; //4. Logical expressions
    BoolExpr *expr[MAX_PREMISES];   //5. Internal logical representation of the premises

    //Game status
    GameRule *rule;
};
typedef struct GameBoard GameBoard;

#endif