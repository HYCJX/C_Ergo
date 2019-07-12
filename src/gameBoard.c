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

//Return if all Premises are valid.
bool buildExprFromPremises(GameBoard *board)
{
    for (int i = 0; i < MAX_PREMISES; i++) {
        bool isValid = true;
        board->expr[i] = buildBoolExpr(board->premise[i], &isValid);
        if (!isValid) {
            for (int j = 0; j <= i; j++) {
                freeBoolExpr(board->expr[j]);
                board->expr[j] = NULL;
            }
            return false;
        }
    }
    return true;
}

//Apply the effects of drawing a Card on a Player and the Deck.
//Return true if the Deck isn't empty.
bool drawCard(Deck *deck, Player *player, int num)
{
    for (int i = 0; i < num; i++) {
        Card *card = drawOneCard(deck);
        player->hand[player->handCtr] = card;
        player->handCtr++;
    }
    return deck->size > 0;
}

/*-Four helper functions for opcodes in execCommand-*/

//1.
//Check if @param opcode is Justification.
static bool isJustificationOpcode(insOpcode opcode)
{
    return (opcode >= OP_JUST && opcode <= OP_WOPJUST);
}

//2.
//Check if @param opcode is in the Utils.
static bool isUtilOpcode(insOpcode opcode)
{
    return (opcode >= OP_DISCARD && opcode < OP_END);
}

//3.
//
static bool parseNumbers(char **tokens, int length, int *dest)
{
    for (int i = 0; i < length; i++) {
        char *buffer;
        dest[i] = (int) strtol(tokens[i], &buffer, 10);
        if (*buffer != '\0') {
            printf("Not a valid number!\n");
            return false;
        }
    }
    return true;
}

//4.
//Return the opcode represented by @param str.
//Return OP_END if not matched.
static insOpcode getInsOpcode(char *str)
{
    if (str == NULL) return OP_END;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char) toupper(str[i]);
    }
    for (insOpcode i = OP_A; i < OP_END; i++) {
        for (int j = 0; opcodeDic[i][j][0] != '\0'; j++) {
            if (strcmp(opcodeDic[i][j], str) == 0) {
                return i;
            }
        }
    }
    return OP_END;
}

//Execute all six command types:
bool execCommand(GameBoard *board, Player *player, char **tokens, bool *isTerminated)
{
    insOpcode opcode = getInsOpcode(tokens[0]);
    int numOfArgs = 0;
    for (numOfArgs = 0; tokens[numOfArgs]; numOfArgs++);
    //Fallacy check:
    if (player->fallacyCtr > 0
        && !isJustificationOpcode(opcode) && !isUtilOpcode(opcode)) {
        printf("You cannot play any cards other than justifications when you have a fallacy!\n");
        return false;
    }
    //DirectPut:
    if (opcode <= OP_PAR) { //All opcodes less than parenthese are directPuts.
        if (numOfArgs == 1) {
            //Directly put onto the first empty premise.
            for (int i = 0; i < MAX_PREMISES; i++) {
                if (board->premise[i]->size == 0) {

                    return execDirectPut(board, player, opcode, i, 0);
                }
            }
            printf("No more empty premises left!\n");
            return false;
        }
        if (numOfArgs == 2) {
            //Directly put to the end of the given premise.
            int index = 0;
            if (parseNumbers(&tokens[1], 1, &index)) {
                if (index >= 0 && index < MAX_PREMISES) {
                    return execDirectPut(board, player, opcode, index, board->premise[index]->size);
                } else {
                    printf("Not a valid premise!\n");
                }
            }
            return false;
        }
        if (numOfArgs == 3) {
            int num[numOfArgs - 1];
            if (!parseNumbers(&tokens[1], 2, num)) {
                return false;
            }
            return execDirectPut(board, player, opcode, num[0], num[1]);
        }
        printf("Wrong number of arguments!\n");
        return false;
    }
    //Tabula Rasa:
    if (opcode == OP_TR) {
        if (numOfArgs != 3) {
            printf("Wrong number of arguments!\n");
            return false;
        }
        int num[numOfArgs - 1];
        if (!parseNumbers(&tokens[1], 2, num)) {
            return false;
        }
        if (num[0] >= 0 && num[0] < MAX_PREMISES) {
            return execTabulaRasa(board, player, num[0], num[1] - 1);
        }
        printf("Not a valid premise!\n");
        return false;

    }
    //Revolution:
    if (opcode == OP_REV) {
        if (numOfArgs != 5) {
            printf("Wrong number of arguments!\n");
            return false;
        }
        int num[numOfArgs - 1];
        if (!parseNumbers(&tokens[1], 4, num)) {
            return false;
        }
        if (num[1] >= 1 && num[3] >= 1) {
            if (num[0] == num[2] && num[1] == num[3]) {
                printf("Revolution on a single card? What ever possessed you?!\n");
                return false;
            }
            return execRevolution(board, player, num[0], num[1] - 1, num[2], num[3] - 1);
        }
        printf("Invalid index\n");
        return false;
    }
    //Fallacy:
    if (opcode == OP_FAL) {
        if (numOfArgs != 2) {
            printf("Wrong number of arguments!\n");
            return false;
        }
        char *target = tokens[1];
        if (strlen(target) == 1) {
            char c = (char) toupper(target[0]);
            if (c < 'A' || c > (board->numOfPlayers - 1 + 'A') || c == player->chosenVar) {
                printf("Not a valid player!\n");
                return false;
            }
            Player *toPlayer = board->player[c - 'A'];
            return execFallacy(board, player, toPlayer);
        }
    }
    //Immediate:
    if (opcode == OP_ERGO || isJustificationOpcode(opcode)) {
        if (numOfArgs != 1) {
            printf("Wrong number of arguments!\n");
            return false;
        }
        return execImmediate(board, player, opcode, isTerminated);
    }
    //Discard:
    if (opcode == OP_DISCARD) {
        if (numOfArgs > 2) {
            printf("Wrong number of arguments!\n");
            return false;
        }
        insOpcode discard = getInsOpcode(tokens[1]);
        if (discard == OP_DISCARD || discard == OP_END) {
            printf("Unable to discard!\n");
            return false;
        }
        return execDiscard(board, player, discard);
    }
    //Help:
    if (opcode == OP_HELP) {
        if (numOfArgs != 1) {
            printf("Wrong number of arguments!\n");
            return false;
        }
        //Play easter egg!
        easterEgg();
        // return false
        return false;
    }
    //Sorthand:
    if (opcode == OP_SORTHAND) {
        if (numOfArgs != 1) {
            printf("Wrong number of arguments!\n");
            return false;
        }
        sortHand(player->hand, player->handCtr, compareCard);
        return false;
    }
    printf("Cannot find instruction!\n");
    return false;
}

//Return a new deeply cloned GameBoard of @param source.
GameBoard *cloneBoard(GameBoard *source)
{
    GameBoard *board = (GameBoard *) calloc(1, sizeof(GameBoard));
    //Clone two counters:
    board->discardIndex = source->discardIndex;
    board->numOfPlayers = source->numOfPlayers;
    //Clone five objects:
    //1. discardPile. 
    for (int i = 0; i < source->discardIndex; i++) {
        board->discardPile[i] = cloneCard(source->discardPile[i]);
    }
    //2. Deck.
    board->deck = cloneDeck(source->deck);
    //3. Players.
    for (int i = 0; i < source->numOfPlayers; i++) {
        board->player[i] = clonePlayer(source->player[i]);
    }
    //4,5. Premises & boolExpr.
    for (int i = 0; i < MAX_PREMISES; i++) {
        board->premise[i] = clonePremise(source->premise[i]);
        board->expr[i] = cloneBoolExpr(source->expr[i]);
    }
    //Clone GameRule.
    board->rule = cloneGameRule(source->rule);
    return board;
}

//A constructor that constructs a new GameBoard from @param rule and @param playerNum.
GameBoard *newBoard(GameRule *rule, int playerNum)
{
    GameBoard *board = (GameBoard *) malloc(sizeof(GameBoard));
    //Two counters:
    board->discardIndex = 0;
    board->numOfPlayers = playerNum;
    //Deck:
    board->deck = initializeDeck(rule->key);
    //Players:
    for (int i = 0; i < playerNum; i++) {
        board->player[i] = newPlayer((char) (i + 'A'));
    }
    //Premises & boolExprs.
    for (int i = 0; i < 4; i++) {
        board->premise[i] = newPremise();
        board->expr[i] = NULL;
    }
    //GameRule:
    board->rule = cloneGameRule(rule);
    printf("Initialize board OK!\n");
    return board;
}

//Applies the effects of disposing a Card on the @param board and the @param plyaer.
void dispose(GameBoard *board, Player *player)
{
    int iter = 0;
    int swapCtr = 5;
    while (iter < 5) {
        if (player->hand[iter] && player->hand[iter]->isDisposed) {
            if (swapCtr >= 7) {
                perror("Disposed more than 3 cards!\n");
            }
            swapCardsInHand(player, iter, swapCtr++);
        } else {
            iter++;
        }
    }
    disposeCard(board, player->hand[5]);
    disposeCard(board, player->hand[6]);
    player->hand[5] = NULL;
    player->hand[6] = NULL;
    player->handCtr = 5;
}

//Store the scores of the Players on the @param board in @param dest.
void extractScores(GameBoard *board, int *dest)
{
    for (int i = 0; i < board->numOfPlayers; i++) {
        dest[i] = board->player[i]->score;
    }
}

void finalEval(GameBoard *board)
{
    //Clear and reprint the Premises:
    clearScreen();
    printf("#     End of this round!    #\n");
    printPremises(board);
    printf("- - - - - - - - - - - - - - -\n");
    //Evaluate the Premises:
    int validAmount = 0;
    int score[4] = {0, 0, 0, 0};
    for (uint32_t code = 0b0000; code <= 0b1111; code++) {
        bool areAllTrue = true;
        for (int exprIndex = 0; exprIndex < MAX_PREMISES; exprIndex++) {
            areAllTrue &= evalBoolExpr(board->expr[exprIndex], code);
        }
        if (areAllTrue) {
            validAmount++;
            for (int i = 0; i < 4; i++) {
                score[i] += (code >> i) & 1;
            }
        }
    }
    //Calculate available scores:
    int numOfCards = 0;
    for (int i = 0; i < MAX_PREMISES; i++) {
        numOfCards += board->premise[i]->size;
    }
    //Calculate scores:
    if (validAmount == 0) { //Paradox:
        printf("This game ends up being a paradox!\n");
        bool isVarOnPremises[4] = {false, false, false, false};
        //Exit if not allow paradox victory
        if (!board->rule->allowParadoxVictory) {
            return;
        }
        //Else:
        //Check if variable is on any Premise:
        for (int i = 0; i < MAX_PREMISES; i++) {
            Premise *p = board->premise[i];
            for (int j = 0; j < p->size; j++) {
                Card *curr = p->card[j];
                if (curr->type == VAR || curr->type == WILD_VAR) {
                    if (curr->CardAs.varName >= 'A' && curr->CardAs.varName <= 'D') {
                        isVarOnPremises[curr->CardAs.varName - 'A'] = true;
                    } else {
                        perror("Unknown varname!\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        //Announce paradox winner:
        for (char c = 'A'; c < ('A' + board->numOfPlayers); c++) {
            if (!isVarOnPremises[c - 'A']) {
                printf("Player [%c] wins this round by paradox victory!\n", c);
                board->player[c - 'A']->score += numOfCards;
            }
        }
    } else { //Find the winner(s):
        bool isOneProven = false;
        for (char c = 'A'; c <= 'D'; c++) {
            if (score[c - 'A'] == validAmount) {
                printf("%c is proven true\n", c);
                isOneProven = true;
                if (c - 'A' < board->numOfPlayers) {
                    board->player[c - 'A']->score += numOfCards;
                }
            }
        }
        if (!isOneProven) {
            printf("No variable can be proven.\n");
        }
    }
}

//Free the space that stores the Aparam board deeply.
void freeBoard(GameBoard *board)
{
    //Sanity check:
    if (board == NULL) return;
    //free discard pile:
    for (int i = 0; i < board->discardIndex; i++) {
        free(board->discardPile[i]);
    }
    //Free Deck:
    freeDeck(board->deck);
    //Free Player's hands + Players themselves:
    for (int i = 0; i < board->numOfPlayers; i++) {
        Player *player = board->player[i];
        for (int j = 0; j < player->handCtr; j++) {
            if (player->hand[j] && !player->hand[j]->isDisposed) {
                free(player->hand[j]);
            }
        }
        free(player);
    }
    //Free Premises and Cards in them:
    for (int i = 0; i < MAX_PREMISES; i++) {
        Premise *p = board->premise[i];
        freePremise(p);
    }
    //Free boolExprs:
    for (int i = 0; i < MAX_PREMISES; i++) {
        freeBoolExpr(board->expr[i]);
    }
    //Free GameRule:
    free(board->rule);
    //Free the gameBoard:
    free(board);
}

//Update the scores of each Player using data from @param source[MAX_PLAYERS].
void updateScores(GameBoard *board, const int source[MAX_PLAYERS])
{
    for (int i = 0; i < board->numOfPlayers; i++) {
        board->player[i]->score = source[i];
    }
}

/*
DEBUG:
int main(void) {
   GameRule *rule = newGameRule();
   GameBoard *board = newBoard(rule, 4);
   printBoardtoPlayer(board, board->player[0]);
   drawCard(board->deck, board->player[0], 7);
   //printf("player->hand0, %s\n", board->player[0]->hand[0] ? "NOT NULL" : "NULL");
   printBoardtoPlayer(board, board->player[0]);
   execDirectPut(board, board->player[0], OP_C, 0, 0);
   execDirectPut(board, board->player[0], OP_PAL, 0, 1);
   dispose(board, board->player[0]);
   printBoardtoPlayer(board, board->player[0]);
   printf("Board is %s !\n", buildExprFromPremises(board) ? "valid" : "invalid");
   return 0;
}
*/