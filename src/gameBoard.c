#include "gameBoard.h"

const char opcodeDic[OP_END][16][64] = {
        {"a",                 "A",       "\0"},
        {"b",                 "B",       "\0"},
        {"c",                 "C",       "\0"},
        {"d",                 "D",       "\0"},

        {"wa",                "WA",      "\0"},
        {"wb",                "WB",      "\0"},
        {"wc",                "WC",      "\0"},
        {"wd",                "WD",      "\0"},

        {"NOT",               "~",       "NEGATE",   "¬",   "\0"},
        {"AND",               "&",       "^",        "\0"},
        {"OR",                "|",       "V",        "\0"},
        {"IMP",               "THEN",    "IMPLIES",  "->",  "\0"},
        {"IFF",               "#",       "<->",      "\0"},

        {"WNOT",              "W~",      "WNEGATE",  "\0"},
        {"WAND",              "W&",      "\0"},
        {"WOR",               "W|",      "WV",       "\0"},
        {"WIMP",              "WTHEN",   "WIMPLIES", "W->", "\0"},

        {"PAL",               "(",       "\0"},
        {"PAR",               ")",       "\0"},

        {"TABULARASA",        "TR",      "\0"},
        {"REVOLUTION",        "REV",     "R",        "\0"},
        {"ERGO",              "ERGO!",   "E",        "\0"},

        {"FALLACY",           "FAL",     "F",        "\0"},
        {"JUSTIFICATION",     "JUST",    "J",        "\0"},
        {"WJUSTIFICATION",    "WJUST",   "WJ",       "\0"},
        {"WVARJUSTIFICATION", "WVJUST",  "WVJ",      "\0"},
        {"WOPJUSTIFICATION",  "WOPJUST", "WOJ",      "\0"},

        {"DISCARD",           "DC",      "/",        "\0"},
        {"HELP",              "?",       "\0"},
        {"SORTHAND",          "SORT",    "S",        "\0"},
        {"CLEARSCREEN",       "CLS",     "\0"}
};

//Helper Functions
static bool isUtilOpcode(insOpcode opcode)
{
    return (opcode >= OP_DISCARD && opcode < OP_END);
}

static bool isJustificationOpcode(insOpcode opcode)
{
    return (opcode >= OP_JUST && opcode <= OP_WOPJUST);
}

static insOpcode getInsOpcode(char *str)
{
    if (str == NULL) return OP_END;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char) toUpper(str[i]);
    }
    for (insOpcode i = OP_A; i < OP_END; i++) {
        for (int j = 0; opcodeDic[i][j][0] != '0'; j++) {
            if (strcmp(opcodeDic[i][j], str) == 0) {
                return i;
            }
        }
    }
    return OP_END;
}

//Functions
bool buildExprFromPremises(GameBoard *board);

bool execCommand(GameBoard *board, Player *player, char **tokens, bool *isTerminated);

GameBoard *newBoard(GameRule *rule, int playerNum);

GameBoard *cloneBoard(GameBoard *source);

void dispose(GameBoard *board, Player *player);

void extractScores(GameBoard *board, int *dest)
{
    for (int i = 0; i < board -> numOfPlayers; i++) {
        dest[i] = board -> player[i] -> score;
    }
}

void finalEval(GameBoard *board);

void freeBoard(GameBoard *board);

void startNewRound(GameBoard *board);

void updateScores(GameBoard *board, const int source[MAX_PLAYERS])
{
    for (int i = 0; i < board -> numOfPlayers; i++) {
        board -> player[i] -> score = source[i];
    }
}