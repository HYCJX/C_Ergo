#include "premise.h"

//Helper functions
static int compare(Operator a, Operator b)
{
    if (a == b) return 0;
}

//Functioning part
bool putCardtoPremise(Premise *premise, Card *card, int index);

bool applyRevolution(Premise *premise, int index);

BoolExpr *buildBoolExpr(Premise *premise, bool *valid);

Card *applyTabulaRasa(Premise *premise, int index);

Premise *newPremise(void)
{
    Premise *premise = malloc(sizeof(Premise));
    premise -> card = malloc(sizeof(Card) * MAX_PREMISE__LENGTH);
    for (int i = 0; i < MAX_PREMISE__LENGTH; i++) {
        premise -> card[i] = NULL;
    }
    premise -> size = 0;
    return premise;
}

Premise *clonePremise(Premise *source);

void printlnPremise(Premise *premise)
{
    for (int i = 0; i < premise -> size; i++) {
        char str[64] = "\0";
        CardToStrInLine(premise -> card[i], str);
        printf("%s", str);
    }
    printf("\n");
}

void freePremise(Premise *premise);
