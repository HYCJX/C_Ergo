#include "gameBoardUtils.h"

//A helper function that checks whether @param card has insOpcode @param opcode.
static bool isOpcodeCard(insOpcode opcode, Card *card)
{
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

//A helper function that finds the Card with insOpcode @param opcode in @param player's hand.
//Return the index if found and -1 if not found.
static int findCardInHand(Player *player, insOpcode opcode)
{
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

/*-----Six Command Types-----*/

//1.
bool execDirectPut(GameBoard *board, Player *player, insOpcode opcode, int premiseId, int index)
{
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
    //Parenthese and Wild cards handling:
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

//2.
bool execDiscard(GameBoard *board, Player *player, insOpcode opcode)
{
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

//3.
bool execFallacy(GameBoard *board, Player *playerFrom, Player *playerTo)
{
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

//4.
bool execImmediate(GameBoard *board, Player *player, insOpcode opcode, bool *isTerminated)
{
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
            //Reset fallacy counter:
            player->fallacyCtr = 0;
            player->isJustified = true;
            play->isDisposed = true;
            printf("You have justified yourself! You are now immune to fallacies!\n");
            return true;
        } else if (opcode == OP_ERGO) {
            //Check if all variables are present:
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

//5.
bool execRevolution(GameBoard *board, Player *player, int premiseId1, int index1, int premiseId2, int index2)
{
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

//6.
bool execTabulaRasa(GameBoard *board, Player *player, int premiseId, int index)
{
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

//Apply the effects of discarding @param card on the @param board.
void disposeCard(GameBoard *board, Card *card)
{
    card->isDisposed = true;
    if (board->discardIndex >= 256) {
        perror("Discard pile out of bound!\n");
        return;
    }
    board->discardPile[board->discardIndex] = card;
    board->discardIndex++;
}