#include "game.h"

static bool isGameTerminated(const int scoreBoard[4], int vicPt) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (scoreBoard[i] >= vicPt) {
            return true;
        }
    }
    return false;
}

int main(void) {
    
    /*---Preparation Phase---*/

    clearScreen();
    char c;
    do {
        c = inputSingleChar("Please input number of players[1 - 4]:\n");
    } while (c <= '0' || c > '4');
    int numOfPlayers = c - '0';
    //Initilization:
    GameRule *rule = newGameRule();
    int scoreBoard[4] = {0, 0, 0, 0};
    //Set rule:
    clearScreen();
    setGameRule(rule);
    //Confirm start
    printf("\n====Initialization successful====\n");
    //Next round confirm:
    printf("    Press ENTER to start: \n");
    getchar();
    printf("\n");

    /*---Game Phase---*/

    bool isForceQuit = false;

    while (!isGameTerminated(scoreBoard, rule->victoryPoint) && !isForceQuit) {
        //Initialize new gameBoard:
        GameBoard *board = newBoard(rule, numOfPlayers);
        //Update score to board:
        updateScores(board, scoreBoard);
        //Opening draw:
        for (int i = 0; i < numOfPlayers; i++) {
            drawCard(board->deck, board->player[i], OPENING_DRAW);
        }
        bool isRoundTerminated = false;
        int playerIndex = 0;
        //Each player's turn:
        while (!isRoundTerminated) {
            Player *currPlayer = board->player[playerIndex];
            //Turn draw:
            drawCard(board->deck, currPlayer, TURN_DRAW);
            clearScreen();
            pauseBeforeSwitchTurn(board, currPlayer, rule->switchPauseDuration);
            //Create temporary view of gameBoard
            GameBoard *view = NULL;
            //Continuously re-run this tun if the view is not well-formed at the end of the turn:
            while (true) {
                freeBoard(view);
                view = cloneBoard(board);
                Player *viewPlayer = view->player[playerIndex];
                //Print gameBoard:
                printBoardtoPlayer(view, viewPlayer);
                //Take 2 actions:
                int numOfValidActions = 0;
                //Each turn:
                while (numOfValidActions < TURN_DRAW && !isRoundTerminated) {
                    bool isValidExec;
                    //Repeat execution:
                    do {
                        printFalIfAvaliable(viewPlayer);
                        //Fetch the instruction from cmdline.
                        printf("Player [%c], Input your command:\n", viewPlayer->chosenVar);
                        size_t size = 128;
                        char *instruction = malloc(sizeof(char) * size);
                        fgets(instruction, size, stdin);
                        //Tokenize the instruction:
                        char **tokens = tokenize(instruction);
                        free(instruction);
                        clearScreen();
                        //Repetitively execute action until valid:
                        isValidExec = execCommand(view, viewPlayer, tokens, &isRoundTerminated);
                        //Reprint after execution:
                        printBoardtoPlayer(view, viewPlayer);
                        //Free tokens:
                        for (int k = 0; tokens[k]; k++) {
                            free(tokens[k]);
                        }
                        free(tokens);
                    } while (!isValidExec);
                    numOfValidActions++;
                    //Reprint:
                    clearScreen();
                    printBoardtoPlayer(view, viewPlayer);
                }
                //Dispose:
                dispose(view, viewPlayer);
                //Check Premises:
                bool isWellFormed = buildExprFromPremises(view);
                if (isWellFormed) {
                    break;
                } else {
                    clearScreen();
                    printf("!Premises are not well formed! Restart this turn!\n");
                }
            }
            //Update fallacy counter:
            if (view->player[playerIndex]->fallacyCtr != 0) {
                view->player[playerIndex]->fallacyCtr += (view->player[playerIndex]->fallacyCtr > 0) ? -1 : 1;
            }
            //Update view to main board:
            freeBoard(board);
            board = view;
            isRoundTerminated |= (board->deck->size == 0);
            //End round if round terminates:
            if (isRoundTerminated) {
                break;
            }
            //Update playerIndex
            playerIndex++;
            playerIndex = playerIndex >= board->numOfPlayers ? 0 : playerIndex;

        }
        //Update scores:
        finalEval(board);
        printScoreBoard(board);
        extractScores(board, scoreBoard);
        freeBoard(board);
        //Next round confirm:
        char yn;
        do {
            yn = (char) toupper(inputSingleChar("Continue playing? [Y / n]:\n"));
        } while(yn != 'Y' && yn != 'N');
        printf("\n");
        isForceQuit = (yn == 'N');

    }

    /*---End Phase---*/

    printf("#==================================#\n");
    printf("            Final Result\n");
    for (int i = 0; i < numOfPlayers; i++) {
        printf(" | Player [%c] -- score: %d!, %s\n", (i + 'A'),
               scoreBoard[i],
               scoreBoard[i] >= rule->victoryPoint ? "Victory!" : "Defeat");
    }
    printf("#==================================#\n");
    //End-game free
    free(rule);
    return EXIT_SUCCESS;
}