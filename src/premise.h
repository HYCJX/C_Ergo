#ifndef _PREMISE_
#define _PREMISE_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "boolExpr.h"
#include "card.h"

bool applyRevolution(Premise *premise1, int index1, Premise *premise2, int index2);

bool putCardtoPremise(Premise *premise, Card *card, int index);

BoolExpr *buildBoolExpr(Premise *premise, bool *valid);

Card *applyTabulaRasa(Premise *premise, int index);

Premise *clonePremise(Premise *source);

Premise *newPremise(void);

void freePremise(Premise *premise);

void printlnPremise(Premise *premise);

#endif