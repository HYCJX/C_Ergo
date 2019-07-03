#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "gameBoard.h"
#include "cardCollection.h"
#include "dataStructures.h"

//const char helpInfo[][] = {
//        "Direct-put Instruction: <opcode> <premiseId> <Index>\n",
//        "Tabula Rasa Instruction: <tabula_rasa> <premiseId> <Index>\n",
//
//};

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

bool isUtilOpcode(insOpcode opcode) {
    return (opcode >= OP_DISCARD && opcode < OP_END);
}

bool isJustificationOpcode(insOpcode opcode) {
    return (opcode >= OP_JUST && opcode <= OP_WOPJUST);
}

insOpcode getInsOpcode(char *str) {
    if (str == NULL) return OP_END;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char) toupper(str[i]);
    }
//    printf("Str: %s\n", str);
    for (insOpcode i = OP_A; i < OP_END; i++) {
        for (int j = 0; opcodeDic[i][j][0] != '\0'; j++) {
            if (strcmp(opcodeDic[i][j], str) == 0) {
                return i;
            }
        }
    }
    return OP_END;
}

void updateScores(GameBoard *board, const int source[MAX_PLAYERS]) {
    for (int i = 0; i < board->numOfPlayers; i++) {
        board->player[i]->score = source[i];
    }
}

void extractScores(GameBoard *board, int *dest) {
    for (int i = 0; i < board->numOfPlayers; i++) {
        dest[i] = board->player[i]->score;
    }
}

GameBoard *newBoard(GameRule *rule, int playerNum) {
    GameBoard *board = (GameBoard *) malloc(sizeof(GameBoard));
    board->rule = cloneGameRule(rule);
    board->numOfPlayers = playerNum;


    for (int i = 0; i < playerNum; i++) {
        board->player[i] = newPlayer((char) (i + 'A'));
    }
    for (int i = 0; i < 4; i++) {
        board->premise[i] = newPremise();
        board->expr[i] = NULL;
    }
    board->deck = initializeDeck(rule->key);

//    board->discardPile = (Card **) malloc(sizeof(Card *) * 256);
    board->discardIndex = 0;

    printf("Initialize board OK!\n");
    return board;
}

/**
 * Clone basically everything from a board.
 * @param source
 * @return
 */
GameBoard *cloneBoard(GameBoard *source) {
    GameBoard *board = (GameBoard *) calloc(1, sizeof(GameBoard));
    board->rule = cloneGameRule(source->rule);
    board->numOfPlayers = source->numOfPlayers;
    for (int i = 0; i < source->numOfPlayers; i++) {
        board->player[i] = clonePlayer(source->player[i]);
    }
    for (int i = 0; i < MAX_PREMISES; i++) {
        board->premise[i] = clonePremise(source->premise[i]);
        board->expr[i] = cloneBoolExpr(source->expr[i]);
    }
    board->deck = cloneDeck(source->deck);

    for (int i = 0; i < source->discardIndex; i++) {
        board->discardPile[i] = cloneCard(source->discardPile[i]);
    }
    board->discardIndex = source->discardIndex;
    return board;
}

bool isOpcodeCard(insOpcode opcode, Card *card) {
    switch (opcode) {
        case OP_A:
        case OP_B:
        case OP_C:
        case OP_D:
            return (card->type == VAR)
                   && card->CardAs.varName == (opcode + ('A' - OP_A));
        case OP_WA:
        case OP_WB:
        case OP_WC:
        case OP_WD:
        case OP_WVJUST:
            return (card->type == WILD_VAR);
//                   && card->CardAs.varName == (opcode + ('A' - OP_WA));
        case OP_NOT:
        case OP_AND:
        case OP_OR:
        case OP_IMP:
        case OP_IFF:
            return (card->type == OP)
                   && card->CardAs.op == (opcode - (OP_NOT - NOT));
        case OP_WNOT:
        case OP_WAND:
        case OP_WOR:
        case OP_WIMP:
        case OP_WOPJUST:
            return (card->type == WILD_OP);
//                   && card->CardAs.op == (opcode - (OP_WNOT - NOT));
        case OP_TR:
            return (card->type == TABULA_RASA);
        case OP_REV:
            return (card->type == REVOLUTION);
        case OP_ERGO:
            return (card->type == ERGO);
        case OP_PAL:
        case OP_PAR:
            return (card->type == PAREN);
        case OP_FAL:
            return (card->type == FALLACY);
        case OP_JUST:
            return (card->type == JUSTIFICATION);
        case OP_WJUST:
            return (card->type == WILD_OP || card->type == WILD_VAR);
        default:
            return false;
    }
}

int findCardInHand(Player *player, insOpcode opcode) {
    for (int i = 0; i < 7; i++) {
        Card *currcard = player->hand[i];
        if (currcard == NULL) {
            perror("Null card found in hand!\n");
            return FIND_FAIL;
        }
        if (!(currcard->isDisposed) && isOpcodeCard(opcode, currcard)) {
            return i;
        }
    }
    printf("Unable to find the card!\n");
    return FIND_FAIL;
}

// 0


void disposeCard(GameBoard *board, Card *card) {
    card->isDisposed = true;

    if (board->discardIndex >= 256) {
        perror("Discard pile out of bound!\n");
        return;
    }
    board->discardPile[board->discardIndex] = card;
    board->discardIndex++;
}

void dispose(GameBoard *board, Player *player) {
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


bool execDirectPut(GameBoard *board, Player *player, insOpcode opcode, int premiseId, int index) {
    if (premiseId >= MAX_PREMISES || premiseId < 0) {
        printf("Not a valid premise!\n");
        return false;
    }
    if (opcode > OP_PAR) {
        printf("You can't put that card to premise directly!\n");
        return false;
    }
    int cardIndex = findCardInHand(player, opcode);
    if (cardIndex == FIND_FAIL) {
        return false;
    }
    Card *play = player->hand[cardIndex];
    // parens & wilds handling
    switch (opcode) {
        case OP_PAL:
            play->CardAs.isLeft = true;
            break;
        case OP_PAR:
            play->CardAs.isLeft = false;
            break;
        case OP_WA:
        case OP_WB:
        case OP_WC:
        case OP_WD:
            play->CardAs.varName = (opcode - OP_WA + 'A');
            break;

        case OP_WNOT:
        case OP_WAND:
        case OP_WOR:
        case OP_WIMP:
            play->CardAs.op = (opcode - OP_WNOT + NOT);
            break;

        default:
            break;
    }
    if (putCardtoPremise(board->premise[premiseId], play, index)) {
        play->isDisposed = true;
        return true;
    }
    printf("You can't put this card over there!\n");
    return false;
}

bool execImmediate(GameBoard *board, Player *player, insOpcode opcode, bool *isTerminated) {
    int cardIndex = findCardInHand(player, opcode);
    if (cardIndex != FIND_FAIL) {
        Card *play = player->hand[cardIndex];
        if (opcode == OP_JUST
            || opcode == OP_WJUST
            || opcode == OP_WVJUST
            || opcode == OP_WOPJUST) {
            if (player->fallacyCtr <= 0) {
                printf("You cannot justify yourself without being fallacied!\n");
                return false;
            }
            // reset fallacy counter
            player->fallacyCtr = 0;
            player->isJustified = true;
            play->isDisposed = true;
            printf("You have justified yourself! You are now immune to fallacies!\n");
            return true;
        } else if (opcode == OP_ERGO) {
            //ergo
            // check if all vars are present
            for (char c = 'A'; c <= 'D'; c++) {
                bool isPresent = false;
                for (int i = 0; i < MAX_PREMISES; i++) {
                    Premise *p = board->premise[i];
                    for (int j = 0; j < p->size; j++) {
                        Card *card = p->card[j];
                        if ((card->type == VAR || card->type == WILD_VAR)
                            && card->CardAs.varName == c) {

                            isPresent = true;
                        }
                    }
                }
                if (!isPresent) {
                    printf("Cannot use ergo here! Variable %c not found!\n", c);
                    return false;
                }
            }
            play->isDisposed = true;
            *isTerminated = true;
            return true;

        } else {
            printf("Not an immediate instruction!\n");
            return false;
        }
    }
    return false;
}

bool execTabulaRasa(GameBoard *board, Player *player, int premiseId, int index) {
    if (premiseId < 0 || premiseId >= MAX_PREMISES) {
        printf("Invalid premise!\n");
        return false;
    }
    if (index < 0 || index > board->premise[premiseId]->size) {
        printf("Not a valid index\n");
        return false;
    }
    int cardIndex = findCardInHand(player, OP_TR);
    if (cardIndex != FIND_FAIL) {
        Card *temp = applyTabulaRasa(board->premise[premiseId], index);
        if (temp != NULL) {
            Card *play = player->hand[cardIndex];
            play->isDisposed = true;
            disposeCard(board, temp);
            return true;
        }
    }
    return false;
}

bool execRevolution(GameBoard *board, Player *player, int premiseId1, int index1, int premiseId2, int index2) {
    if (premiseId1 < 0 || premiseId1 > 3 || premiseId2 < 0 || premiseId2 > 3) {
        printf("Invalid premise ID!\n");
        return false;
    }
    int cardIndex = findCardInHand(player, OP_REV);
    if (cardIndex != FIND_FAIL) {
        Card *play = player->hand[cardIndex];
        if (applyRevolution(
                board->premise[premiseId1], index1,
                board->premise[premiseId2], index2)
                ) {
            play->isDisposed = true;
            return true;
        }
    }
    return false;
}

bool execFallacy(GameBoard *board, Player *playerFrom, Player *playerTo) {
    int cardIndex = findCardInHand(playerFrom, OP_FAL);
    if (cardIndex != FIND_FAIL) {
        Card *play = playerFrom->hand[cardIndex];
        if (playerTo->fallacyCtr != 0 || playerTo->isJustified) {
            printf("You cannot apply fallacy to this player!\n");
            return false;
        }
        playerTo->fallacyCtr = board->rule->fallacyPenalty;
        play->isDisposed = true;
        return true;
    }
    return false;
}

bool execDiscard(GameBoard *board, Player *player, insOpcode opcode) {
    int cardIndex = findCardInHand(player, opcode);
    if (cardIndex != FIND_FAIL) {
        Card *play = player->hand[cardIndex];
        if (play->isDisposed) {
            perror("The player tried to discard a disposed card!\n");
            return false;
        }
        play->isDisposed = true;
        return true;
    }
    return false;
}

bool buildExprFromPremises(GameBoard *board) {
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


void freeBoard(GameBoard *board) {
    //sanity check
    if (board == NULL) return;

    // free premises and cards in them
    for (int i = 0; i < MAX_PREMISES; i++) {
        Premise *p = board->premise[i];
        freePremise(p);
    }

    //free deck
    freeDeck(board->deck);

    // free boolExprs
    for (int i = 0; i < MAX_PREMISES; i++) {
        freeBoolExpr(board->expr[i]);
    }

    //free discard pile
    for (int i = 0; i < board->discardIndex; i++) {
        free(board->discardPile[i]);
    }


    // then free player's hands + players themselves
    for (int i = 0; i < board->numOfPlayers; i++) {
        Player *player = board->player[i];
        for (int j = 0; j < player->handCtr; j++) {
            if (player->hand[j] && !player->hand[j]->isDisposed) {
                free(player->hand[j]);
            }
        }
        free(player);
    }
    // then free game rule
    free(board->rule);

    //finally free the board itself
    free(board);
}

bool parseNumbers(char **tokens, int length, int *dest) {
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


bool execCommand(GameBoard *board, Player *player, char **tokens, bool *isTerminated) {
    insOpcode opcode = getInsOpcode(tokens[0]);
    int numOfArgs = 0;
    for (numOfArgs = 0; tokens[numOfArgs]; numOfArgs++);

    //fallacy check
    if (player->fallacyCtr > 0
        && !isJustificationOpcode(opcode) && !isUtilOpcode(opcode)) {
        printf("You cannot play any cards other than justifications when you have a fallacy!\n");
        return false;
    }

    if (opcode <= OP_PAR) { //All opcodes less than par are direct put
        if (numOfArgs == 1) {
            // direct put onto the first empty premise.
            for (int i = 0; i < MAX_PREMISES; i++) {
                if (board->premise[i]->size == 0) {

                    return execDirectPut(board, player, opcode, i, 0);
                }
            }
            printf("No more empty premises left!\n");
            return false;
        }

        if (numOfArgs == 2) {
            // direct put to the end of the given premise.
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
    if (opcode == OP_ERGO || isJustificationOpcode(opcode)) {
        if (numOfArgs != 1) {

            printf("Wrong number of arguments!\n");
            return false;
        }
        return execImmediate(board, player, opcode, isTerminated);
    }

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

    if (opcode == OP_HELP) {
        if (numOfArgs != 1) {
            printf("Wrong number of arguments!\n");
            return false;
        }

        // play easter egg!
        easterEgg();
        // return false
        return false;
    }

    if (opcode == OP_SORTHAND) {
        if (numOfArgs != 1) {
            printf("Wrong number of arguments!\n");
            return false;
        }

//        printf("Unsupported instruction sry!\n");
        sortHand(player->hand, player->handCtr, compareCard);
//        printBoardtoPlayer(board, player);
        return false;
    }


    printf("Cannot find instruction!\n");
    return false;
}

void finalEval(GameBoard *board) {
    //clear and reprint premise
    clearScreen();
    printf("#     End of this round!    #\n");
    printPremises(board);
    printf("- - - - - - - - - - - - - - -\n");

    int validAmount = 0;
    int score[4] = {0, 0, 0, 0};
    for (uint32_t code = 0b0000; code <= 0b1111; code++) {
        bool areAllTrue = true;
        for (int exprIndex = 0; exprIndex < MAX_PREMISES; exprIndex++) {
            areAllTrue &= evalBoolExpr(board->expr[exprIndex], code);
        }
//        printf("with code: %d, premise is %d\n", code, evalRes);
        if (areAllTrue) {
            validAmount++;
            for (int i = 0; i < 4; i++) {
                score[i] += (code >> i) & 1;
            }
        }
    }

    // calculate score
    int numOfCards = 0;
    for (int i = 0; i < MAX_PREMISES; i++) {
        numOfCards += board->premise[i]->size;
    }

    if (validAmount == 0) {
        printf("This game ends up being a paradox!\n");
        bool isVarOnPremises[4] = {false, false, false, false};

        //exit if not allow paradox victory
        if (!board->rule->allowParadoxVictory) {
            return;
        }

        // check if var is not on any premise
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

        for (char c = 'A'; c < ('A' + board->numOfPlayers); c++) {
            if (!isVarOnPremises[c - 'A']) {
                printf("Player [%c] wins this round by paradox victory!\n", c);
                board->player[c - 'A']->score += numOfCards;
            }
        }

    } else {

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

////DEBUG
//int main(void) {
//    GameRule *rule = newGameRule();
//    GameBoard *board = newBoard(rule, 4);
//    printBoardtoPlayer(board, board->player[0]);
//    drawCard(board->deck, board->player[0], 7);
////    printf("player->hand0, %s\n", board->player[0]->hand[0] ? "NOT NULL" : "NULL");
//    printBoardtoPlayer(board, board->player[0]);
//    execDirectPut(board, board->player[0], OP_C, 0, 0);
//    execDirectPut(board, board->player[0], OP_PAL, 0, 1);
//    dispose(board, board->player[0]);
//    printBoardtoPlayer(board, board->player[0]);
//
//    printf("Board is %s !\n", buildExprFromPremises(board) ? "valid" : "invalid");
//
//    return 0;
//}