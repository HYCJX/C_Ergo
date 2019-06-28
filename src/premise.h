#ifndef _PREMISE_
#define _PREMISE_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "dataStructures.h"
#include "boolExpr.h"
#include "card.h"

bool putCardtoPremise(Premise *premise, Card *card, int index);

bool applyRevolution(Premise *premise1, int index1, Premise *premise2, int index2);

BoolExpr *buildBoolExpr(Premise *premise, bool *valid);

Card *applyTabulaRasa(Premise *premise, int index);

Premise *newPremise(void);

Premise *clonePremise(Premise *source);

void printlnPremise(Premise *premise);

void freePremise(Premise *premise);

#endif