#include "cardCollection.h"

//Helper functions
static Deck *newDeck(void)
{
    Deck *deck = malloc(sizeof(Deck));
    deck -> head = NULL;
    deck -> tail = NULL;
    deck -> size = 0;
    return deck;
}

static DeckNode *newDeckNode(Card * card)
{
    DeckNode *node = malloc(sizeof(DeckNode));
    node -> card = card;
    node -> next = NULL;
    return node;
}

static void addCardtoTail(Deck *deck, Card *card)
{
    DeckNode *node = newDeckNode(card);
    if (deck -> size == 0) {
        deck -> head = node;
        deck -> tail = node;
    } else {
        deck -> tail -> next = node;
        deck -> tail = node;
    }
    deck -> size++;
}

static void resetTail(Deck *deck)
{
    DeckNode *node = deck -> head;
    if (node == NULL) return;
    for (; node -> next; node = node -> next);
    deck -> tail = node;
}

//Functions
Deck *initializeDeck(const int key[NUM_OF_CARD_TYPE])
{
    int varNum = key[0];
    int notNum = key[1];
    int andNum = key[2];
    int orNum = key[3];
    int impNum = key[4];
    int iffNum = key[5];
    int parenNum = key[6];
    int trNum = key[7];
    int revNum = key[8];
    int wVarNum = key[9];
    int wOpNum = key[10];
    int fjNum = key[11];
    int ergoNum = key[12];
    int total = 0;
    for (int i = 1; i < NUM_OF_CARD_TYPE; total += key[i++]);
    total += varNum * 4;
    total += fjNum;
    printf("%D\n", total);
    Card *tempCardPool[total];
    int index = 0;
    //var cards
    for (int i = 0; i < varNum; i++) {
        for (char c = 'A'; c <= 'D'; c++) {
            tempCardPool[index++] = newCard(c);
        }
    }

    //Build temp pool
    for (int i = 0; i < andNum; i++) {
        tempCardPool[index++] = newOpCard(AND);
    }

    for (int i = 0; i < orNum; i++) {
        tempCardPool[index++] = newOpCard(OR);
    }
    for (int i = 0; i < impNum; i++) {
        tempCardPool[index++] = newOpCard(IMPLIES);
    }
    for (int i = 0; i < notNum; i++) {
        tempCardPool[index++] = newOpCard(NOT);
    }
    for (int i = 0; i < parenNum; i++) {
        tempCardPool[index++] = newSpecialCard(PAREN);
    }
    for (int i = 0; i < trNum; i++) {
        tempCardPool[index++] = newSpecialCard(TABULA_RASA);
    }
    for (int i = 0; i < revNum; i++) {
        tempCardPool[index++] = newSpecialCard(REVOLUTION);
    }
    for (int i = 0; i < wVarNum; i++) {
        tempCardPool[index++] = newSpecialCard(WILD_VAR);
    }
    for (int i = 0; i < wOpNum; i++) {
        tempCardPool[index++] = newSpecialCard(WILD_OP);
    }
    for (int i = 0; i < ergoNum; i++) {
        tempCardPool[index++] = newSpecialCard(ERGO);
    }
    for (int i = 0; i < fjNum; i++) {
        tempCardPool[index++] = newSpecialCard(FALLACY);
        tempCardPool[index++] = newSpecialCard(JUSTIFICATION);
    }
    printf("Initializing deck\n");
    int seed = (int) time(NULL);
    if (DEBUG) {
        printf("DEBUG: seed = %d\n", seed);
    }
    sprand(seed);
    Deck *deck = newDeck();
    for (int i = total - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        addCardtoTail(deck, tempCardPool[j]);
        tempCardPool[j] = tempCardPool[i];
    }
    printf("Initialize deck OK!\n");
    return deck;
}

Deck *cloneDeck(Deck *source)
{
    if (source == NULL) return NULL;
    Deck *deck = newDeck();
    if (source -> size == 0) return deck;
    DeckNode *curr = source -> head;
    deck -> head = newDeckNode(cloneCard(curr -> card));
    DeckNode *pointer = deck -> head;
    curr = curr -> next;
    while (curr) {
        pointer -> next = newDeckNode(cloneCard(curr -> card));
        pointer = pointer -> next;
        curr = curr -> next; 
    }
    resetTail(deck);
    deck -> size = source -> size;
    return deck;
}

Card *drawOneCard(Deck *deck)
{
    if (deck -> size == 0) return NULL;
    DeckNode *tmp = deck -> head;
    Card *card = tmp -> card;
    deck -> head = deck -> head -> next;
    deck -> size--;
    free(tmp);
    return card;
}

void addCardtoTail(Deck *deck, Card *card)
{
    DeckNode * node = newDeckNode(card);
    if (deck -> size == 0) {
        deck -> head = node;
        deck -> tail = node;
    } else {
        deck -> tail -> next = node;
        deck -> tail = node;
    }
    deck -> size++;
}

void freeDeck(Deck *deck)
{
    while (deck -> size > 0) {
        free(drawOneCard(deck));
    }
    free(deck);
}