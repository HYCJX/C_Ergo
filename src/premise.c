#include "premise.h"

//Helper functions
static int compare(Operator a, Operator b)
{
    if (a == b) return 0;
}

//WHERE IS IFF ??? ???
//WHY ALL *VALID IS FALSE ???
static BoolExpr *buildhelper(Premise *premise, int start, int end, bool *valid)
{
    if (start == end && (premise -> card[start] -> type == VAR || premise -> card[start] -> type == WILD_VAR)) {
        return newVariableExpr(premise -> card[start] -> CardAs.varName);
    } else if (start < end) {
        Card **card = premise -> card;
        Operator op = NO_OPERATOR;
        int indexOfOp = -1;   //index of the most significant operator (MSO)
        int indexOfFLP = -1;  //index of first left parenthesis
        int indexOfFMRP = -1; //index of first matched right parenthesis
        
        //Find and update MSO and the three indices:
        int parenCounter = 0;
        for (int i = start; i <= end; i++) {
            CardType type = card[i] -> type;
            //Parentheses dealing:
            if (type == PAREN) {
                bool isLeft = card[i] -> CardAs.isLeft;
                //Initialize index for the first left parenthesis: 
                if (indexOfFLP == -1 && isLeft) {
                    indexOfFLP = i;
                }
                //Update the value of parenCounter:
                parenCounter += isLeft ? 1 : -1;
                //Initialize index for the matched right parenthesis of the first left parenthesis:
                if (indexOfFMRP == -1 && parenCounter == 0) {
                    indexOfFMRP = i;
                }
            }
            if (parenCounter == 0) {
                if ((type == OP || type == WILD_OP) && card[i] -> CardAs.op > op) {
                    op = card[i] -> CardAs.op;
                    indexOfOp = i;
                }
            }
        }

        //If the expression looks like: (<exp>) and the first and the last parentheses are matched, evaluate <exp>.
        if (indexOfFLP == start && indexOfFMRP == end) {
            return buildHelper(premise, start + 1, end -1, valid);
        }

        //Check op:
        switch (op) {
            case NOT:
            //If NOT is the MSO, then the form of the premise is: ~<var> or ~(<exp>).
                if (indexOfOp == start) {
                    CardType nextCardType = card[indexOfOp + 1] -> type;
                    if (nextCardType == VAR || nextCardType == WILD_VAR || nextCardType == PAREN) {
                        return newExpr(NOT, buildHelper(premise, indexOfOp + 1, end, valid), newTrueExpr());
                    }
                }
                //Else there is an error in type.
                printf("NOT error.\n");
                break;
            case AND:
            case OR:
            case IMPLIES:
                return newExpr(op, buildHelper(premise, start, indexOfOp - 1, valid), buildHelper(premise, indexOfOp + 1, end, valid));
            default:
                break;
        }
    }
    *valid = false;
    return newDummyExpr;
}

//Functions

//Index 0 means in front of all cards.
//Index n means after the (n - 1)th card in the premise.
bool putCardtoPremise(Premise *premise, Card *card, int index)
{
    if (index < 0 || index > premise -> size) return false;
    for (int i = premise -> size; i >= index; i--) {
        premise -> card[i] = premise -> card[i - 1];
    }
    premise -> card[index] = card;
    premise -> size++;
    return true;
}

//NEED TO FREE ??? ???
bool applyRevolution(Premise *premise1, int index1, Premise *premise2, int index2)
{
    if (index1 >= premise1 -> size || index2 >=  premise2 -> size) {
        printf("The index is out of bound.\n");
        return false;
    }
    Card *card1 = premise1 -> card[index1];
    Card *card2 = premise2 -> card[index2];
    if (card1 && card2 && card1 -> type == card2 -> type) {
        Card *temp = card1;
        premise1 -> card[index1] = card2;
        premise2 -> card[index2] = temp;
        return true;
    }
    printf("Two cards are not of the same type!\n");
    return false;
}

//Build boolean expression from premise if possible and return the evaluated boolExpr.
BoolExpr *buildBoolExpr(Premise *premise, bool *valid)
{
    if (premise -> size == 0) {
        *valid = true;
        return newTrueExpr();
    }
    int parenCounter = 0;
    for (int i = 0; i < premise -> size; i++) {
        if (premise -> card[i] -> type == PAREN) {
            parenCounter += premise -> card[i] -> CardAs.isLeft ? 1 : -1;
        }
        if (parenCounter < 0) {
            printf("One right parenthesis cannot find its other half!\n");
            *valid = false;
            return newDummyExpr();
        }
    }
    if (parenCounter == 0) {
        return buildhelper(premise, 0, premise -> size - 1, valid);
    }
    printf("Unpaired parentheses! Not a valid premise!\n");
    *valid = false;
    return newDummyExpr();
}

//Remove the given variable card and return the pointer of the card.
Card *applyTabulaRasa(Premise *premise, int index)
{
    if (premise == NULL ||index >= premise -> size) {
        printf("The index is out of bound.\n");
        return NULL;
    }
    Card *result = premise -> card[index];
    for (int i = index + 1; i < premise -> size; i++) {
        premise -> card[i - 1] = premise -> card[i];
    }
    premise -> size--;
    premise -> card[premise -> size] = NULL;
    return result;
}

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

Premise *clonePremise(Premise *source)
{
    if (source == NULL) return NULL;
    Premise *result = newPremise();
    for (int i = 0; i < MAX_PREMISE__LENGTH; i++) {
        result -> card[i] = cloneCard(source -> card[i]);
    }
    result -> size = source -> size;
    return result;
}

void printlnPremise(Premise *premise)
{
    for (int i = 0; i < premise -> size; i++) {
        char str[64] = "\0";
        CardToStrInLine(premise -> card[i], str);
        printf("%s", str);
    }
    printf("\n");
}

//WHY NOT FREE EVERY CARD ???
void freePremise(Premise *premise)
{
    if (premise == NULL) return;
    free(premise -> card);
    free(premise);
}
