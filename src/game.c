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

    clearScreen();

    char c;
    do {
        c = inputSingleChar("Please input number of players[1 - 4]:\n");
    } while (c <= '0' || c > '4');
    int numOfPlayers = c - '0';

    //Init
    GameRule *rule = newGameRule();
    int scoreBoard[4] = {0, 0, 0, 0};
    // set rule
    clearScreen();
    setGameRule(rule);

//    // confirm start
    printf("\n====Initialization successful====\n");
    //next round confirm:
    printf("    Press ENTER to start: \n");
    getchar();
    printf("\n");

//    printf("%d\n", board->numOfPlayers);
    //Game Phase
    bool isForceQuit = false;

    while (!isGameTerminated(scoreBoard, rule->victoryPoint) && !isForceQuit) {

        // init new board
        GameBoard *board = newBoard(rule, numOfPlayers);
        // update score to board
        updateScores(board, scoreBoard);

        //Opening draw
        for (int i = 0; i < numOfPlayers; i++) {
            drawCard(board->deck, board->player[i], OPENING_DRAW);
        }
        bool isRoundTerminated = false;
        int playerIndex = 0;


        // each player's turn
        while (!isRoundTerminated) {
            Player *currPlayer = board->player[playerIndex];
            //turn draw
            drawCard(board->deck, currPlayer, TURN_DRAW);

            clearScreen();
            pauseBeforeSwitchTurn(board, currPlayer, rule->switchPauseDuration);

            // create temporary view of board
            GameBoard *view = NULL;

            // continuously re-run this tun if the view is not well-formed at the end of the turn.
            while (true) {
                freeBoard(view);
                view = cloneBoard(board);
                Player *viewPlayer = view->player[playerIndex];

                //print board
                printBoardtoPlayer(view, viewPlayer);
                //Take 2 action
                int numOfValidActions = 0;

                // Each turn
                while (numOfValidActions < TURN_DRAW && !isRoundTerminated) {

                    bool isValidExec;
                    // repeat exec
                    do {

                        printFalIfAvaliable(viewPlayer);
                        //fetch instruction from cmdline
                        printf("Player [%c], Input your command:\n", viewPlayer->chosenVar);
                        size_t size = 128;
                        char *instruction = malloc(sizeof(char) * size);
                        fgets(instruction, size, stdin);
                        //tokenize instruction
                        char **tokens = tokenize(instruction);
                        free(instruction);

                        clearScreen();

                        //repeatedly execute action until valid
                        isValidExec = execCommand(view, viewPlayer, tokens, &isRoundTerminated);

                        //reprint after execution
                        printBoardtoPlayer(view, viewPlayer);

                        //free tokens
                        for (int k = 0; tokens[k]; k++) {
                            free(tokens[k]);
                        }
                        free(tokens);

                    } while (!isValidExec);
                    numOfValidActions++;

                    //reprint
                    clearScreen();
                    printBoardtoPlayer(view, viewPlayer);

                }

                //dispose
                dispose(view, viewPlayer);

                bool isWellFormed = buildExprFromPremises(view);
                if (isWellFormed) {

                    break;

                } else {

                    clearScreen();
                    printf("!Premises are not well formed! Restart this turn!\n");

                }

            }

            // update fallacy counter
            if (view->player[playerIndex]->fallacyCtr != 0) {
                view->player[playerIndex]->fallacyCtr += (view->player[playerIndex]->fallacyCtr > 0) ? -1 : 1;
            }

            //update view to main board.
            freeBoard(board);
            board = view;

            isRoundTerminated |= (board->deck->size == 0);

            //end round if round terminates
            if (isRoundTerminated) {
                break;
            }

            //update playerIndex
            playerIndex++;
            playerIndex = playerIndex >= board->numOfPlayers ? 0 : playerIndex;

        }

        // update scores
        finalEval(board);
        printScoreBoard(board);

        extractScores(board, scoreBoard);

        freeBoard(board);


        //next round confirm:
        char yn;
        do {
            yn = (char) toupper(inputSingleChar("Continue playing? [Y / n]:\n"));
        } while(yn != 'Y' && yn != 'N');
        printf("\n");
        isForceQuit = (yn == 'N');

    }
    //End Phase

    printf("#==================================#\n");
    printf("            Final Result\n");
    for (int i = 0; i < numOfPlayers; i++) {
        printf(" | Player [%c] -- score: %d!, %s\n", (i + 'A'),
               scoreBoard[i],
               scoreBoard[i] >= rule->victoryPoint ? "Victory!" : "Defeat");
    }
    printf("#==================================#\n");

    // end-game free
    free(rule);


    return EXIT_SUCCESS;
}