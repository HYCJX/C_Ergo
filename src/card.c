#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "card.h"
#include "dataStructures.h"

static const char *OP_TO_STR[] = {"NO", "~", "&", "V", "->"};

Card *newVarCard(const char c) {
    Card *card = malloc(sizeof(Card));
    card->type = VAR;
    card->CardAs.varName = c;
    card->isDisposed = false;
    return card;
}

Card *newOpCard(Operator op) {
    Card *card = malloc(sizeof(Card));
    card->type = OP;
    card->CardAs.op = op;
    card->isDisposed = false;
    return card;
}

Card *newSpecialCard(CardType type) {
    Card *card = malloc(sizeof(Card));
    card->type = type;
    card->isDisposed = false;
    return card;
}

void opToStr(Operator op, char *dest) {
    strcpy(dest, OP_TO_STR[op]);
}

void cardToStrInLine(Card *card, char *dest) {
    switch (card->type) {
        case VAR: {
            char temp[2] = "\0";
            temp[0] = card->CardAs.varName;
            strcpy(dest, temp);
            break;
        }
        case OP: {
            opToStr(card->CardAs.op, dest);
            break;
        }
        case PAREN: {
            char temp[2] = "\0";
            temp[0] = (char) (card->CardAs.isLeft ? '(' : ')');
            strcpy(dest, temp);
            break;
        }
        case WILD_VAR: {
            char temp[3] = "* ";
            temp[1] = card->CardAs.varName;
            strcpy(dest, temp);
            break;
        }
        case WILD_OP: {
            char temp[5] = "*";
            char tempOp[3] = "\0\0";
            opToStr(card->CardAs.op, tempOp);
            strcat(&temp[1], tempOp);
            strcpy(dest, temp);
            break;
        }
        case TABULA_RASA:
            break;
        case REVOLUTION:
            break;
        case FALLACY:
            break;
        case JUSTIFICATION:
            break;
        case ERGO:
            break;
        case NO_TYPE:
            break;
    }
}

void cardToStr(Card *card, char *dest) {
    char temp[2];
    switch (card->type) {
        case VAR:
            temp[0] = card->CardAs.varName;
            temp[1] = '\0';
            strcpy(dest, temp);
            break;
        case OP:
            opToStr(card->CardAs.op, dest);
            break;
        case PAREN:
            strcpy(dest, "(");
            break;
        case WILD_VAR:
            strcpy(dest, "wV");
            break;
        case WILD_OP:
            strcpy(dest, "wO");
            break;
        case TABULA_RASA:
            strcpy(dest, "TR");
            break;
        case REVOLUTION:
            strcpy(dest, "R");
            break;
        case ERGO:
            strcpy(dest, "E");
            break;
        case FALLACY:
            strcpy(dest, "F");
            break;
        case JUSTIFICATION:
            strcpy(dest, "J");
            break;
        default:
            break;
    }
}

Card *cloneCard(Card *card) {
    if (card == NULL) return NULL;
    Card *result = malloc(sizeof(Card));
    result->type = card->type;
//    switch (result->type) {
//        case VAR:
//            result->CardAs.varName = card->CardAs.varName;
//            break;
//        case OP:
//            result->CardAs.op = card->CardAs.op;
//            break;
//        case PAREN:
//            result->CardAs.isLeft = card->CardAs.isLeft;
//            break;
//    }
    result->CardAs = card->CardAs;
    result->isDisposed = card->isDisposed;

    return result;

}

int compareCard(Card *c1, Card *c2) {
    if (c1 == NULL) return 1;
    if (c2 == NULL) return -1;
    if (c1->type < c2->type) return -1;
    if (c1->type > c2->type) return 1;
    // assert c1->type = c2->type
    switch (c1->type) {
        case VAR:
            if (c1->CardAs.varName > c2->CardAs.varName) return 1;
            if (c1->CardAs.varName < c2->CardAs.varName) return -1;
            return 0;
        case OP:
            if (c1->CardAs.op > c2->CardAs.op) return 1;
            if (c1->CardAs.op < c2->CardAs.op) return -1;
            return 0;
        default:
            break;
    }
    return 0;
}

void sortHand(Card *hand[], int size, int (*cmp)(Card *, Card *)) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int res = cmp(hand[i], hand[j]);
            if (res > 0) {
                Card *tmp = hand[i];
                hand[i] = hand[j];
                hand[j] = tmp;
            }
        }
    }
}