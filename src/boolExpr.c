#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "boolExpr.h"

BoolExpr *newVariableExpr(const char name) {
    BoolExpr *expr = malloc(sizeof(BoolExpr));
    expr->variable = name;
    expr->op = NO_OPERATOR;
    expr->leftExpr = NULL;
    expr->rightExpr = NULL;
    return expr;
}

BoolExpr* newExpr(Operator op, BoolExpr *left, BoolExpr *right) {
    BoolExpr *expr = malloc(sizeof(BoolExpr));
    expr->variable = '\0';
    expr->op = op;
    expr->leftExpr = left;
    expr->rightExpr = right;
    return expr;
}

BoolExpr* newDummyExpr() {
    BoolExpr *expr = malloc(sizeof(BoolExpr));;
    expr->variable = '\0';
    expr->op = NO_OPERATOR;
    expr->leftExpr = NULL;
    expr->rightExpr = NULL;
    return expr;
}

BoolExpr* newTrueExpr() {
    BoolExpr *expr = malloc(sizeof(BoolExpr));;
    expr->variable = '\0';
    expr->op = OR;
    expr->leftExpr = newExpr(NOT, newVariableExpr('A'), newDummyExpr());
    expr->rightExpr = newVariableExpr('A');
    return expr;
}

//Evaluate boolean expr in accordance to the given evalCode, with its last 4 digit goes: DCBA;
bool evalBoolExpr(BoolExpr *expr, uint32_t evalCode) {
    if (expr->variable != '\0') {
        return (bool) ((evalCode >> (expr->variable - 'A')) & 1);
    }
    switch (expr->op) {
        case NOT: return !evalBoolExpr(expr->leftExpr, evalCode);
        case AND: return evalBoolExpr(expr->leftExpr, evalCode) & evalBoolExpr(expr->rightExpr, evalCode);
        case OR: return evalBoolExpr(expr->leftExpr, evalCode) | evalBoolExpr(expr->rightExpr, evalCode);
        case IMPLIES: return !evalBoolExpr(expr->leftExpr, evalCode) | evalBoolExpr(expr->rightExpr, evalCode);
        default: perror("Evaluation Error! Unknown operator!\n");
    }
    return false;
}

bool boolExpToStr(BoolExpr *expr, char *dest) {
    if (expr->variable != '\0') {
        char resultBuffer[2] = "\0";
        resultBuffer[0] = expr->variable;
        strcpy(dest, resultBuffer);
        return true;
    }
    char resultBuffer[512] = "(", leftBuffer[512];
    if (boolExpToStr(expr->leftExpr, leftBuffer)) {
        if (expr->op == NOT) {
            strcat(resultBuffer, "~");
            strcat(resultBuffer, leftBuffer);
            strcat(resultBuffer, ")");
            strcpy(dest, resultBuffer);
            return true;
        }
        char rightBuffer[512];
        if (boolExpToStr(expr->rightExpr, rightBuffer)) {
            char op[3] = "\0\0";
            switch(expr->op) {
                case AND: strcpy(op, "&"); break;
                case OR: strcpy(op, "V"); break;
                case IMPLIES: strcpy(op, "->"); break;
                default: perror("Evaluation Error! Unknown operator!\n"); return false;
            }
            strcat(resultBuffer, leftBuffer);
            strcat(resultBuffer, op);
            strcat(resultBuffer, rightBuffer);
            strcat(resultBuffer, ")");
            strcpy(dest, resultBuffer);
            return true;
        }
    }
    return false;
}

void freeBoolExpr(BoolExpr *expr) {
    if (expr != NULL) {
        freeBoolExpr(expr->rightExpr);
        freeBoolExpr(expr->leftExpr);
        free(expr);
    }
}

BoolExpr *cloneBoolExpr(BoolExpr *source) {
    if (source == NULL) {
        return NULL;
    }
    if (source->variable != '\0') {
        return newVariableExpr(source->variable);
    }
    return newExpr(source->op, cloneBoolExpr(source->leftExpr), cloneBoolExpr(source->rightExpr));
}
//DEBUG:

// int main(void) {
//     BoolExpr *a = newVariableExpr('A');
//     BoolExpr *b = newVariableExpr('B');
//     BoolExpr *c = newVariableExpr('C');
//     BoolExpr *aANDc = newExpr(AND, a, c);
//     BoolExpr *aANDcIMPb = newExpr(IMPLIES, aANDc, b);
//     BoolExpr *final = newExpr(IMPLIES, b, aANDcIMPb);
//
//     char str[512];
//     boolExpToStr(final, str);
//     printf("%s\n", str);
//
//     uint32_t code1 = 0b1100;
//     uint32_t code2 = 0b0001;
//
//     printf("code1: %d, code2: %d\n", evalBoolExpr(final, code1), evalBoolExpr(final, code2));
//
//     freeBoolExpr(final);
//     final = NULL;
//
//     return EXIT_SUCCESS;
//
// }