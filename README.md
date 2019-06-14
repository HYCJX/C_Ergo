# Project: cErgo ∴

#### Introduction
__cErgo__ is a re-implementation of the propositional-logic-based board game _ERGO_.  
__cErgo__ is written in C only.  
Detailed rules of vanilla _ERGO_ game can be found in _Ergo Rules 2015.pdf_, or [here](https://www.catalystgamelabs.com/download/Ergo%20Rules%202015.pdf).  

#### cErgo Rules
__cErgo__ has its own rules instead of the vanilla _ERGO_ game rules (but we do support vanilla rules).  

1. __cErgo__ has the same deck of cards as _ERGO_.  
2. In general, __cErgo__ rules are spproximately the same as vanilla _ERGO_ rules. Which means:
    - Each player needs to prove their variables while trying to disproving others'.
    - Each player starts with 5 cards.
    - Each player draws 2 cards, and play at most 2 cards in each turn. The player needs to discard his/her cards until he/she has 5 cards after his/her turn.
    - Direct double negations ```~~A``` is not allowed. Double negation can only be used with the aid of parentheses ```~(~A)```.
    - If the player who draws the last card from the deck played a _tabula rasa_ card in his turn. Although the deck is not empty, the round terminates immediately after this player's round (or if he plays a _Ergo_ card).
3. In __cErgo__, priorities of logical operators are strictly deduced according to propositional logic rules.
    - We also support vanilla rules.
4. __cErgo__ hates _fallacies_ and _justifications_! We __strongly__ recommend you play games without _fallacies_ and _justifications_.  
5. If a round ends up being a paradox, players whose variables are all discarded wins this round.


#### cErgo cmd line instructions
__cErgo__ can be played via GUI or command line.
Syntax of __cErgo__ can be found here:

1.  Direct-put Instruction: ```<opcode> <premiseId> <Index>```
    -   This instruction will check if the player has the specified variable or operator card. If the player is able to play        this card, place a card from the player's hand to the given position specified by the premiseId and Index.
    -   Avaliable ```<opcode>``` may include:
        -   variables:  ```A, B, C, D```
        -   operators:  ```NOT, AND, OR, IMPLIES, IFF```
        -   operator symbols:  ```~, &, |, V, ->, <->, #```
        -   operator aliases:  ```NEGATE (NOT), THEN (IMPLIES), IMP (IMPLIES)```
        -   parentheses:  ```paL, paR```
        -   parenthesis symbols:  ```(, )```
2.  Tabula Rasa Instruction: ```<tabula_rasa> <premiseId> <Index>```
    -   This instruction has the same syntax as Direct-put, but instead of putting cards into the premise, it executes the effect of _tabula rasa_ card, which removes the given card at the given index and put it at the bottom of the deck.
    -   Avaliable ```<tabula_rasa>``` may include:
        -   tabula rasa: ```TABULARASA```
        -   tabula rasa symbol: ```TR```
3.  Revolution Instruction: ```<revolution> <premiseId1> <Index1> <premiseId2> <Index2>```
    -   This instruction executes the _revolution_ card's effect, which first check whether the given two cards are of the same type, then if yes, swap them.
    -   Avaliable ```<revolution>``` may include:
        -   revolution: ```REVOLUTION```
        -   revolution abbreviation: ```REV```
4.  Fallacy Instruction: ```<fallacy> <player_name>```
    -   This instruction plays _fallacy_ card (Oh I hate it...) to a player, if the given player is not the player him/herself.
    -   Avaliable ```<fallacy>``` may include:
        -   fallacy: ```FALLACY```
        -   fallacy abbreviation: ```FAL```
5.  Immediate Instruction: ```<imm>```
    -   This instruction plays _Justification_ card on the player him/herself, or plays _ERGO_ if necessary.
    -   Avaliable ```<imm>``` may include:
        -   justification: ```JUSTIFICATION```
        -   justification abbreviation: ```JUST```
        -   ergo: ```ERGO```
        -   ergo with excitement: ```ERGO!```
6.  Discard instruction: ```discard <card>```
    -   This instruction allows the player to discard his card.
    -   Avaliable ```<card>``` may include:
        -   variables:  ```A, B, C, D```
        -   wild-card variables:  ```Wvar```
        -   operators:  ```NOT, AND, OR, IMPLIES, IFF```
        -   wild-card operators:  ```Wop```
        -   operator symbols:  ```~, &, |, V, ->, <->, #```
        -   operator aliases:  ```NEGATE (NOT), THEN (IMPLIES), IMP (IMPLIES)```
        -   parenthesis:  ```pa```
        -   parenthesis symbols:  ```(, )```
        -   tabula rasa: ```TABULARASA```
        -   tabula rasa symbol: ```TR```
        -   revolution: ```REVOLUTION```
        -   revolution abbreviation: ```REV```
        -   fallacy: ```FALLACY```
        -   fallacy abbreviation: ```FAL```
        -   justification: ```JUSTIFICATION```
        -   justification abbreviation: ```JUST```
        -   ergo: ```ERGO```
        -   ergo with excitement: ```ERGO!```