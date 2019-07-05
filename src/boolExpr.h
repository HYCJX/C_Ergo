#ifndef _BOOLEAN_EXPRESSION_
#define _BOOLEAN_EXPRESSION_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "dataStructures.h"

bool boolExpToStr(BoolExpr *expr, char *dest);

bool evalBoolExpr(BoolExpr *expr, uint32_t evalCode);

BoolExpr *cloneBoolExpr(BoolExpr *source);

BoolExpr *newDummyExpr();

BoolExpr *newExpr(Operator op, BoolExpr *left, BoolExpr *right);

BoolExpr *newTrueExpr();

BoolExpr *newVariableExpr(const char name);

void freeBoolExpr(BoolExpr *expr);

#endif