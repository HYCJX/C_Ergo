#ifndef _BOOLEAN_EXPRESSION_
#define _BOOLEAN_EXPRESSION_

#include <stdint.h>
#include <stdbool.h>
#include "dataStructures.h"

BoolExpr *newVariableExpr(const char name);

BoolExpr *newExpr(Operator op, BoolExpr *left, BoolExpr *right);

BoolExpr *newDummyExpr();

BoolExpr *newTrueExpr();

BoolExpr *cloneBoolExpr(BoolExpr *source);

bool evalBoolExpr(BoolExpr *expr, uint32_t evalCode);

bool boolExpToStr(BoolExpr *expr, char *dest);

void freeBoolExpr(BoolExpr *expr);

#endif