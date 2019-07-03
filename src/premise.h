#ifndef _PREMISE_
#define _PREMISE_

#include <stdint.h>
#include <stdbool.h>
#include "card.h"
#include "boolExpr.h"
#include "dataStructures.h"

bool putCardtoPremise(Premise *premise, Card *card, int index);

Card *applyTabulaRasa(Premise *premise, int index);

bool applyRevolution(Premise *premise1, int index1, Premise *premise2, int index2);

Premise *newPremise(void);

void printlnPremise(Premise *premise);

BoolExpr *buildBoolExpr(Premise *premise, bool *valid);

Premise *clonePremise(Premise *source);

void freePremise(Premise *premise);


#endif