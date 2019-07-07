#include "premise.h"

//Apply the effects of the revolution Card to the Premises.
//Return true if the application is successful.
bool applyRevolution(Premise *premise1, int index1, Premise *premise2, int index2)
{
    if (index1 >= premise1->size || index2 >= premise2->size) {
        printf("Index out of bound\n");
        return false;
    }
    Card *card1 = premise1->card[index1];
    Card *card2 = premise2->card[index2];
    if (card1 && card2 && card1->type == card2->type) {
        Card *temp = card1;
        premise1->card[index1] = card2;
        premise2->card[index2] = temp;
        return true;
    }
    printf("Two cards are not of the same type!\n");
    return false;
}

//Apply the effects of putting a Card to a Premise at the given index.
//Index 0 means in front of all cards. Index n means after the (n - 1)th card in the premise.
//Return true if the application is successsful.
bool putCardtoPremise(Premise *premise, Card *card, int index)
{
    if (index < 0 || index > premise->size) return false;
    for (int i = premise->size; i >= index; i--) {
        premise->card[i] = premise->card[i - 1];
    }
    premise->card[index] = card;
    premise->size++;
    return true;
}

//A helper function that builds BoolExpr recursively according to the formation tree of the Premise.
static BoolExpr *buildHelper(Premise *premise, int start, int end, bool *valid)
{
    if (start == end && (premise->card[start]->type == VAR || premise->card[start]->type == WILD_VAR)) {
        // If start == end and the evaluated card is a variable card.
        return newVariableExpr(premise->card[start]->CardAs.varName);
    } else if (start < end) {
        Card **card = premise->card;
        Operator op = NO_OPERATOR;  //Most significant operator.
        int indexOfOp = -1;         //Index of the MS operator.
        int indexOfFLP = -1;        //First left parenthesis.
        int indexOfFMRP = -1;       //matched right parenthesis of the 1st left parenthesis.
        //Find and update MS operator and its index:
        int parensCounter = 0;
        for (int i = start; i <= end; i++) {
            CardType type = card[i]->type;
            //paren dealing
            if (type == PAREN) {
                bool isLeft = card[i]->CardAs.isLeft;
                //init index for first left paren...
                if (indexOfFLP == -1 && isLeft) {
                    indexOfFLP = i;
                }
                //update value of parencounter...
                parensCounter += isLeft ? 1 : -1;
                // if it is the matched right paren of the 1st left paren...
                if (indexOfFMRP == -1 && parensCounter == 0) {
                    indexOfFMRP = i;
                }
            }
            if (parensCounter == 0) {
                if ((type == OP || type == WILD_OP)
                    && card[i]->CardAs.op > op) {
                    op = card[i]->CardAs.op;
                    indexOfOp = i;
                }
            }
        }
        // If the expression looks like : (<exp>), and the 1st and last parens are matched, evaluate <exp>.
        if (indexOfFLP == start && indexOfFMRP == end) {
            return buildHelper(premise, start + 1, end - 1, valid);
        }
        //Check op to see if 
        switch (op) {
            case NOT:
                // IF not is the MS op, then the form of the premise is: ~<var> OR ~(<exp>)
                if (indexOfOp == start) {
                    CardType nextCardType = card[indexOfOp + 1]->type;
                    // Now we DISallow double negation here.
                    if (nextCardType == VAR || nextCardType == WILD_VAR || nextCardType == PAREN) {
                        return newExpr(NOT,
                                       buildHelper(premise, indexOfOp + 1, end, valid),
                                //Does not matter what to put on the rhs but just in case.
                                       newTrueExpr());
                    }
                }
                printf("NOT error\n");
                break;
            case AND:
            case OR:
            case IMPLIES:
                return newExpr(op,
                               buildHelper(premise, start, indexOfOp - 1, valid),
                               buildHelper(premise, indexOfOp + 1, end, valid));
                //NO_OPERATOR
            default:
                break;
        }
    }
    //Error handling
    // printf("Invalid premise!\n");
    *valid = false;
    return newDummyExpr();
}

//Build boolean expression from premise if possible and store the evaluated boolExpr into the dest.
//Return a dummy expression if the expression cannot be build (premise is not valid).
BoolExpr *buildBoolExpr(Premise *premise, bool *valid)
{
    if (premise->size == 0) {
        *valid = true;
        return newTrueExpr();
    }
    int parenCounter = 0;
    for (int i = 0; i < premise->size; i++) {
        if (premise->card[i]->type == PAREN) {
            parenCounter += premise->card[i]->CardAs.isLeft ? 1 : -1;
        }
        if (parenCounter < 0) {
            printf("One right parenthesis cannot find its other half! Feelsbadman!\n");
            *valid = false;
            return newDummyExpr();
        }
    }
    if (parenCounter == 0) {
        return buildHelper(premise, 0, premise->size - 1, valid);
    }
    printf("Unpaired parentheses! Not a valid premise!\n");
    *valid = false;
    return newDummyExpr();
}

//Apply the effects of the tabula rasa Card to the Premises.
//Remove the given variable card and store the pointer to that card in dest.
//Return false if the designated card is not a variable card.
Card *applyTabulaRasa(Premise *premise, int index)
{
    // sanity check
    if (premise == NULL || index >= premise->size) {
        printf("Index out of bound\n");
        return NULL;
    }
    int i;
    Card *result = premise->card[index];
    for (i = index + 1; i < premise->size; i++) {
        premise->card[i - 1] = premise->card[i];
    }
    premise->size--;
    premise->card[premise->size] = NULL;
    return result;
}

//Return a new deeply cloned Premise of @param source.
Premise *clonePremise(Premise *source)
{
    if (source == NULL) return NULL;
    Premise *result = newPremise();
    for (int i = 0; i < MAX_PREMISE_LENGTH; i++) {
        result->card[i] = cloneCard(source->card[i]);
    }
    result->size = source->size;
    return result;
}

//A constructor that constructs a new Premise.
Premise *newPremise(void)
{
    Premise *premise = malloc(sizeof(Premise));
    premise->card = malloc(sizeof(Card) * MAX_PREMISE_LENGTH);
    for (int i = 0; i < MAX_PREMISE_LENGTH; i++) {
        premise->card[i] = NULL;
    }
    premise->size = 0;
    return premise;
}

//Free the space that stores a Premise freely.
void freePremise(Premise *premise)
{
    if (premise == NULL) return;
    //  for(int i = 0; i < premise->size; i++) {
    //      free(premise->card[i]);
    //      premise->card[i] = NULL;
    //  }
    free(premise->card);
    free(premise);
}

//Print a Premise.
void printlnPremise(Premise *premise)
{
    for (int i = 0; i < premise->size; i++) {
        char str[64] = "\0";
        cardToStrInLine(premise->card[i], str);
        printf("%s ", str);
    }
    printf("\n");
}

/*
DEBUG

int main(void) {
   Premise *premise[] = {newPremise(), newPremise(), newPremise(), newPremise()};
//    Card *a1 = newVarCard('A');
   Card *a = newVarCard('A');
   Card *b = newVarCard('B');
   Card *c = newVarCard('C');
//    Card *d = newVarCard('D');
   Card *and = newOpCard(AND);
   Card *not = newOpCard(NOT);
   Card *or = newOpCard(OR);
   Card *imp = newOpCard(IMPLIES);
//    Card *imp1 = newOpCard(IMPLIES);
   Card *paL = newSpecialCard(PAREN);
   paL->CardAs.isLeft = true;
   Card *paR = newSpecialCard(PAREN);
   paR->CardAs.isLeft = false;
   Card *wA = newSpecialCard(WILD_VAR);
   wA->CardAs.varName = 'A';
   Card *wOR = newSpecialCard(WILD_OP);
   wOR->CardAs.op = OR;

   Card* cards[4][64] = {
       {b, imp, c, and, a, imp, b},
       {a, or, not, a},
       {a, or, not, a},
       {a, or, not, a}
   };

   //{b, imp, c, and, a, imp, b} -> b is not valid.
   int cardslength[] = {7, 4, 4, 4};
   for (int i = 0; i < 4; i++) {
       for (int j = 0; j < cardslength[i]; j++) {
           putCardtoPremise(premise[i], cards[i][j], premise[i]->size);
       }
   }
   BoolExpr *expr[4];
   bool valid = true;
   for (int i = 0; i < 4; i++) {
       printlnPremise(premise[i]);
       expr[i] = buildBoolExpr(premise[i], &valid);
       printf("Is valid?: %d\n", valid);
   }
   char buffer[512];
   boolExpToStr(expr[0], buffer);
   printf("Bool expr to the first expr: %s\n", buffer);
   BoolExpr *cluster = newExpr(AND,
       newExpr(AND, expr[0], expr[1]),
       newExpr(AND, expr[2], expr[3])
   );
   if (valid) {
       int score[] = {0,0,0,0};
       int validAmount = 0;
       for (int code = 0b0000; code <= 0b1111; code++) {
           bool evalRes = evalBoolExpr(cluster, code);
           printf("with code: %d, premise is %d\n", code, evalRes);
           if (evalRes) {
               validAmount++;
               for (int i = 0; i < 4; i++) {
                   score[i] += (code >> i) & 1;
               }
           }
       }
       printf("------------------\n");
       if (validAmount == 0) {
           printf("Paradox\n");
       } else {
          bool isOneProven = false;
           for (char c = 'A'; c <= 'D'; c++) {
              if (score[c - 'A'] == validAmount) {
                   printf("%c is proven to be true\n", c);
                   isOneProven = true;
               }
           }
           if (!isOneProven) {
               printf("No one is proven.\n");
           }
       }
   }
   // printf("------------------\n");
   // printf("The following should all be invalid\n");
   // Card *invalidCards[6][64] = {
   //     {paR, a, and, b, paL},
   //     {not, or, a, and, b},
   //     {imp, a},
   //     {a, and, b, paL, paR},
   //     {wA, and, or, wA},
   //     {paL, a, paR, paL, b, paR}
   // };
   // int invCardsLength[] = {5, 5, 2, 5, 4, 6};
   // for (int i = 0; i < 6; i++) {
   //     Premise *p = newPremise();
   //     for (int j = 0; j < invCardsLength[i]; j++) {
   //         putCardtoPremise(p, invalidCards[i][j], p->size);
   //     }
   //     printlnPremise(p);
   //     bool foo = true;
   //     BoolExpr *e = buildBoolExpr(p, &foo);
   //     printf("%s", foo ? "valid\n" : "not valid\n");
   // }
   // printf("------------------\n");
   // printf("The following should all be valid\n");
   // Card *validCards[3][64] = {
   //     //A -> ( ~ ( w_A V B ) -> ( C & A ) V C )
   //     {a, imp, paL, not, paL, wA, or, b, paR, imp, paL, c, and, a, paR, or, c, paR},
   //     {paL, a, and, not, b, paR, and, paL, a, imp, b, paR},
   //     {paL, a, and, b, paR}
   // };
   // int vCardsLength[] = {18, 12, 5};
   // for (int i = 0; i < 3; i++) {
   //     Premise *p = newPremise();
   //     for (int j = 0; j < vCardsLength[i]; j++) {
   //         putCardtoPremise(p, validCards[i][j], p->size);
   //     }
   //     printlnPremise(p);
   //     bool foo = true;
   //     BoolExpr *e = buildBoolExpr(p, &foo);
   //     printf("%s", foo ? "valid\n" : "not valid\n");
   // }


   freeBoolExpr(cluster);
   for (int i = 0; i < 4; ++i) {
       freePremise(premise[i]);
   }
   return 0;
}
*/