#include "IOUtils.h"

//Tokenize the given string by space, comma or \n.
//Return the pointer to the result string array.
//The given string it self will NOT be modified.
//@Param *line: The given string.
char **tokenize(char *line)
{
    char *rest = (char *) malloc(strlen(line) + 1);
    char *anchor = rest;
    strcpy(rest, line);
    char **result = (char **) malloc(sizeof(char *));
    if (result) {
        size_t n = 1;
        char *token = strtok_r(rest, " ,\n", &rest);
        while (token) {
            char **tmp = realloc(result, (++n) * sizeof(char *));
            if (tmp == NULL) break;
            result = tmp;
            result[n - 2] = malloc(strlen(token) + 1);
            if (result[n - 2] != NULL) {
                strcpy(result[n - 2], token);
            }
            token = strtok_r(NULL, " ,\n", &rest);
        }
        result[n - 1] = NULL;
    }
    free(anchor);
    return result;
}

//Read a single char after printing a line of @param reminder.
//Return the char.
char inputSingleChar(const char *reminder)
{
    size_t tmpSize = 16;
    char tmpStr[128];
    do {
        printf("%s", reminder);
        fgets(tmpStr, tmpSize, stdin);
    } while (strlen(tmpStr) != 2);
    return tmpStr[0];
}

//Clear the screen.
void clearScreen()
{
    for (int i = 0; i < 3; i++) {
        printf("\e[1;1H\e[2J");
    }
}

//Counts down from @param time to 0.
void countDown(int time)
{
    if (time <= 0) return;
    for (int i = time; i > 0; i--) {
        printf("%d...\n", i);
        sleep(1);
    }
}

//Start a countDown between two players' turn.
void pauseBeforeSwitchTurn(GameBoard *board, Player *player, int time)
{
    // time <= 0 then no pause.
    if (time <= 0) return;
    clearScreen();
    printPremises(board);
    printf("Player [%c]'s turn in: \n", player->chosenVar);
    countDown(3);
    clearScreen();
}

//Print fallacy penalty to @param player if it can be applied.
void printFalIfAvaliable(Player *player)
{
    if (player->fallacyCtr > 0) {
        printf("!!FALLACY, cannot play cards after %d rounds!!\n", player->fallacyCtr);
    }
}

//Print the score board on the screen.
void printScoreBoard(GameBoard *board) {
    printf("---------SCORE BOARD---------\n");
    for (int i = 0; i < board->numOfPlayers; i++) {
        printf("|  Player %c: %dpt   \n", (i + 'A'), board->player[i]->score);
    }
    printf("---------SCORE BOARD---------\n");
}

//Print the Premises.
void printPremises(GameBoard *board) {
    printf("------------Premises------------\n");
    for (int i = 0; i < 4; i++) {
        printf("[%d]: ", i);
        printlnPremise(board->premise[i]);
    }
}

//Print @param board to @param player.
void printBoardtoPlayer(GameBoard *board, Player *player) {
    printPremises(board);
    printf("------------Your hand------------\n");
    printf(" ");
    for (int i = 0; i < 7; i++) {
        if (player->hand[i] && !(player->hand[i]->isDisposed)) {
            char str[16];
            cardToStr(player->hand[i], str);
            printf("%s ", str);
        }
    }
    printf("\n");
    printf("                |%d cards in Deck|\n", board->deck->size);
    printf("---------------------------------\n");
}

/*-----Special Rules-----*/
void easterEgg() {
    printf("                     cErgo -- version %s                         \n", VERSION_NAME);
    printf("|  Imperial College London DoC 1st year computing group project.   |\n");
    printf("|    By Zhige Yu, Caini Liu, Shiyuan Feng & Hongyuan Yan.          |\n");
    printf("|  Thanks to Prof.Russo, Prof.Hodkinson, Prof.Drossopoulou and     |\n");
    printf("|  Dr.Wheelhouse For their intriguing and inspiring logic courses. |\n");
    printf("|  Thanks to all supporters and players of cErgo, including...     |\n");
    printf("|      YOU! :)                                                     |\n");
    printf("Back to game in: \n");
    countDown(7);
    clearScreen();
}

void ruleHelpNotification() {
    printf("cErgo -- version %s -- rule codes:\n", VERSION_NAME);
    printf("\n");
    printf("  DD: Double decker\n");
    printf("  HD: Half decker\n");
    printf("  NOFJ: No fallacy and justification\n");
    printf("  QS: Remove switch player pause\n");
    printf("  APV: Allow paradox victory\n");
    printf("  RFP: Fallacy penalty is reduced to 2\n");
    printf("\n");
    printf("  VANILLA: Reset game rule to vanilla\n");
    printf("  CERGO: Use recommended cErgo rule\n");
    printf("\n");
}