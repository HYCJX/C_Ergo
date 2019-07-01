#include "gameUtils.h"

//Helper function
static void clearScreen() {
    for (int i = 0; i < 3; i++) {
        printf("\e[1;1H\e[2J");
    }
}

//Function
void easterEgg() {
    printf("                     cErgo -- version %s                         \n", VERSION_NAME);
    printf("|  Imperial College London DoC 1st year computing group project.   |\n");
    printf("|    By Zhige Yu, Caini Liu, Shiyuan Feng & Hongyuan Yan.          |\n");
    printf("|  Thanks to Prof.Russo, Prof.Hodkinson, Prof.Drossopoulou and     |\n");
    printf("|  Dr.Wheelhouse For their intriguing and inspiring logic courses. |\n");
    printf("|  Thanks to all supporters and players of cErgo, including...     |\n");
    printf("|      YOU! :)                                                     |\n");
    printf("Back to game in: \n");
    countdown(7);
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
