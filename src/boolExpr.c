#include "boolExpr.h"

//Single variables;
BoolExpr *newVariableExpr(const char name)
{
    BoolExpr *expr = malloc(sizeof(BoolExpr));
    expr -> variable = name;
    expr -> op = NO_OPERATOR;
    expr -> leftExpr = NULL;
    expr -> rightExpr = NULL;
    return expr;
}

BoolExpr *newExpr(Operator op, BoolExpr *left, BoolExpr *right)
{
    BoolExpr * expr = malloc(sizeof(BoolExpr));
    expr -> variable = '\0';
    expr -> op = op;
    expr -> leftExpr = left;
    expr -> rightExpr = right;
    return expr;
}

//For unary operators;
BoolExpr *newDummyExpr()
{
    BoolExpr *expr = malloc(sizeof(BoolExpr));
    expr -> variable = '\0';
    expr -> op = NO_OPERATOR;
    expr -> leftExpr = NULL;
    expr -> rightExpr = NULL;
    return expr;
}

//A or not A;
BoolExpr *newTrueExpr()
{
    BoolExpr *expr = malloc(sizeof(BoolExpr));
    expr -> variable = '\0';
    expr -> op = OR;
    expr -> leftExpr = newExpr(Not, newVariableExpr('A'), newDummyExpr());
    expr -> rightExpr = newVariableExpr('A');
    retur expr;
}

BoolExpr *cloneBoolExpr(BoolExpr *source)
{
    if (source == NULL) {
        return NULL;
    }
    if (source -> variable != '\0') {
        return newVariableExpr(source -> variable);
    }
    return newExpr(source => op, cloneBoolExpr(source -> leftExpr), cloneBoolExpr(source -> rightExpr));
}

//Evaluate boolean expr in accordance to the given avalCode, with its last 4 digit goes: DCBA;
bool evalBoolExpr(BoolExpr *expr, u32int_t evalCode)
{
    if (expr -> variable != '\0') {
        return (bool) ((evalCode) >> (expr -> variable - 'A') & 1);
    }
    switch (expr -> op) {
        case NOT: return !evalBoolExpr(expr -> leftExpr, evalCode);
        case AND: return evalBoolExpr(expr -> leftExpr, evalCode) & evalBoolExpr(expr -> rightExpr, evalCode);
        case OR: return evalBoolExpr(expr -> leftExprm, evalCode) | evalBoolExpr(expr -> rightExpr, evalCode);
        case IMPLIES: return !evalBoolExpr(expr -> leftExprm, evalCode) | evalBoolExpr(expr -> rightExpr, evalCode);
        default: perror("Evaluation Error! Unknown operator!\n");
    }
    return false;
}

bool boolExpToStr(BoolExpr *expr, char *dest)
{
    if (expr -> variable != '\0') {
        char resultBuffer[2] = "\0";
        resultBuffer[0] = expr -> variable;
        strcpy(dest, resultBuffer);
        return true;
    }
    char resultBuffer[512] = "(", left Buffer[512];
    if (boolExpToStr(Expr -> leftExpr, leftBuffer)) {
        if (expr -> op == NOT) {
            strcat(resultBuffer, "~");
            strcat(resultBuffer, leftBuffer);
            strcat(resultBufffer, ")");
            strcpy(dest, resultBuffer);
            return true;
        }
        char rightBuffer[512];
        if (boolExpToStr(expr -> rightExpr, rightBuffer)) {
            char op[3] = "\0\0";
            switch (expr -> op) {
                case AND: strcpy(op, "&"); break;
                case OR: strcpy(op, "|"); break;
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

void freeBoolExpr(BoolExpr *expr)
{
    if (expr != NULL) {
        if (expr -> leftExpr) freeBoolExpr(expr -> leftExpr);
        if (expr -> rightExpr) freeBoolExpr(expr -> rightExpr);
        free(expr);
    }
}