#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cs50.h>
#include "blackjack.h"

int main(void){
    Card *deck = createDeck();
    if(deck == NULL){
        fprintf(stderr, "Deck not found.\n");
        return 1;
    }

    clear();
    greet();


    Card *compHand = NULL;
    Card *playerHand = NULL;

    int newGame = 0;
    do{
        int currWager = wager();

        clear();
        compHand = dealHand(&deck);
        playerHand = dealHand(&deck);
        int action = 0;
        int didBust = 0;
        do{
            clear();
            printStatus(currWager, playerHand, compHand, 1);

            action = requestAction();

            if(action == 1){
                Card *tmp = dealCard(&deck);
                moveCardToDeck(&tmp, &playerHand);
            }
            didBust = checkBust(playerHand);
        }while(action != 2  && didBust == 0);
        clear();
        int win = checkWin(playerHand, compHand);
        while(win == 1 && handValue(compHand) < 17 ){
            Card *tmp = dealCard(&deck);
            moveCardToDeck(&tmp, &compHand);
            win = checkWin(playerHand, compHand);
        }

        printStatus(currWager, playerHand, compHand, 0);

        switch(win){
            case -1:
                printf("You busted and lost.\n");
                funds -= currWager;
                break;
            case 0:
                printf("House won.\n");
                funds -= currWager;
                break;
            case 1:
                printf("You won!\n");
                funds += (2*currWager);
                break;
            case 2:
                printf("You tied with the house. No funds removed.\n");
                break;
        }
        newGame = requestNewGame();
        moveHandToDeck(&playerHand, &deck);
        moveHandToDeck(&compHand, &deck);
        didBust = 0;
    }while(newGame != 2  && funds > 0);
    clear();
    if(funds <= 0){
        printf("You ran out of funds! Please start again with more.\n");
    }
    printf("Thank you for playing Blackjack!\n");

    freeDeck(deck);
    return 0;
}

//Greet the player when we start the game.
void greet(){
    printf("\n\nWelcome to Blackjack.\n\n");

    while(funds <= 0){
        printf("How much money would like you to start with? ");
        funds = get_int();

        if(funds <= 0){
            printf("You must start with some amount greater than 0.\n");
        }
    }

    printf("Fantastic! Let's get started.\n\n");

}

//Request a wager.
int wager(){
    int retVal;
    do{
        printf("How much would you care to wager?      Current Funds: %d\n", funds);
        retVal = get_int();

        if(retVal > funds){
            printf("You cannot wager more than you have. Let's try again.\n");
        }
        else if(retVal <= 0){
            printf("Come on now, you have to wager something.\n");
        }

    }while(retVal < 1 || retVal > funds);

    return retVal;
}

//Create the initial deck as a linked list and return the head.
Card* createDeck(){
    Card *head = NULL;
    Card *current = NULL;
    Card *previous = NULL;

    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 4; j++){
            current = malloc(sizeof(Card));
            current->value = i + 1;
            current->name = cardNames[i];
            current->suit = (Suits)j;

            if(previous != NULL){
                current->prevCard = previous;
                previous->nextCard = current;
            }

            if(head == NULL){
                head = current;
            }
            previous = current;
        }
    }

    return head;
}

//Free the memory from the deck
void freeDeck(Card *deck){

    if(deck->nextCard != NULL){
        freeDeck(deck->nextCard);
    }
    free(deck);
}

//Use this to clear the screen between prompts.
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

//Deal a card from the deck.
Card* dealCard(Card **deck){
    srand(time(NULL));
    int size = deckSize(*deck);
    int rnd = (rand()%size);

    Card *retVal;
    retVal = *deck;
    for(int i =0; i< rnd; i++){
        retVal = retVal->nextCard;
    }
    if(retVal->prevCard == NULL){
        *deck=retVal->nextCard;
    }else{
        retVal->prevCard->nextCard = retVal->nextCard;
    }
    if(retVal->nextCard != NULL){
        retVal->nextCard->prevCard = retVal->prevCard;
    }

    retVal->nextCard = NULL;
    retVal->prevCard = NULL;
    return retVal;
}

Card* popCard(Card **deck){
    Card *retVal;
    retVal = *deck;
    *deck = (*deck)->nextCard;

    retVal->nextCard = NULL;
    retVal->prevCard = NULL;

    return retVal;
}

void printStatus(int wager, Card *playerHand, Card *compHand, int hidden){
    printf("Funds: %i\n", funds);
    printf("Wager: %i\n\n\n", wager);
    printf("Comp Hand:\n");
    printHand(compHand, hidden);
    printf("Player Hand:\n");
    printHand(playerHand, 0);
}

//Print some representation of the card
void printCard(Card* card){
    printf("---------\n");
    printf("|%c      |\n", card->name[0]);
    printf("|       |\n");
    printf("|   %c   |\n", suitLabels[card->suit]);
    printf("|       |\n");
    printf("|      %c|\n", card->name[0]);
    printf("---------\n");
}

void printHiddenCard(){
    printf("---------\n");
    for(int i = 0; i < 5; i++){
        printf("|#######|\n");
    }
    printf("---------\n");
}

int deckSize(Card *deck){
    int retVal = 0;
    Card *curr = deck;

    while(curr != NULL){
        retVal++;
        curr = curr->nextCard;
    }

    return retVal;
}

//Create an array of pointers to cards in a deck
Card* deckToArray(Card* deck){
    int count = deckSize(deck);
    Card *currCard = deck;
    Card* retVal = malloc(count * sizeof(Card                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           ));
    for(int i = 0; i < count; i++){
        retVal[i] = *currCard;
        currCard = currCard->nextCard;
    }
    return retVal;
}

void moveCardToDeck(Card **card, Card **deck){
    (*card)->nextCard = *deck;
    if(*deck != NULL){
        (*deck)->prevCard = *card;
    }
    *deck = *card;
}

void moveHandToDeck(Card **card, Card **deck){
    while((*card) != NULL){
        Card *tmp = popCard(card);
        moveCardToDeck(&tmp, deck);
    }
}

Card* dealHand(Card **deck){
    Card *retVal = NULL;

    Card *tmp;
    for(int i = 0; i< 2; i++){
        tmp = dealCard(deck);
        moveCardToDeck(&tmp, &retVal);
    }

    return retVal;
}

void printHand(Card* hand, int hidden){
    if(hand == NULL){
        printf("No hand to print.\n");
    }else{
        int count = deckSize(hand);
        Card *currCard = hand;
        Card *arr = malloc(count * sizeof(Card));
        for(int i = 0; i < count; i++){
            arr[i] = *currCard;
            currCard = currCard->nextCard;
        }

        for(int i = 0; i < count; i++){
            printf("----------   ");
        }
        printf("\n");

        for(int i =0; i < count; i++){
            if(i < hidden){
                printf("|########|   ");
            }
            else{
                if(arr[i].value == 10){
                    printf("|%s      |   ", arr[i].name);
                }else{
                    printf("|%s       |   ", arr[i].name);
                }
            }
        }
        printf("\n");
        for(int i =0; i < count; i++){
            if(i < hidden){
                printf("|########|   ");
            }
            else{
                printf("|        |   ");
            }
        }
        printf("\n");
        for(int i =0; i < count; i++){
            if(i < hidden){
                printf("|########|   ");
            }
            else{
                printf("|   %c    |   ", suitLabels[arr[i].suit]);
            }
        }
        printf("\n");
        for(int i =0; i < count; i++){
            if(i < hidden){
                printf("|########|   ");
            }
            else{
                printf("|        |   ");
            }
        }
        printf("\n");
        for(int i =0; i < count; i++){
            if(i < hidden){
                printf("|########|   ");
            }
            else{
                if(arr[i].value == 10){
                    printf("|      %s|   ", arr[i].name);
                }else{
                    printf("|       %s|   ", arr[i].name);
                }
            }
        }
        printf("\n");
        for(int i = 0; i < count; i++){
            printf("----------   ");
        }
        printf("\n");

        free(arr);
        /*
        int index = 0;
        Card *tmp = hand;
        while(tmp != NULL){
            if(index < hidden){
                printHiddenCard();
            }else{
                printCard(tmp);
            }
            index++;
            tmp = tmp->nextCard;
        }
        */
    }
}

//Request what action the user woud like to take
int requestAction(){
    int retVal = 0;
    while(retVal < 1 || retVal > 2){
        printf("Actions:\n1. Hit\n2. Stay\nWhat would you like to do?\n");
        retVal = get_int();
        if(retVal != 1 && retVal != 2){
            printf("Thats not a valid action. Please try again.\n");
        }
    }

    return retVal;
}

int requestNewGame(){
    int retVal = 0;
    do{
        printf("Would you like to play another game?     Funds: %i\n 1. Yes\n 2. No\n",funds);
        retVal = get_int();
        if(retVal != 1 && retVal != 2){
            clear();
            printf("I'm sorry I didn't catch that.\n");
        }
    }while(retVal != 1 && retVal != 2);

    return retVal;
}

int handValue(Card *hand){
    int hard = hardValue(hand);
    int soft = softValue(hand);

    if(hard > 21)
    {
        return soft;
    }else{
        return hard;
    }
}

int softValue(Card *hand){
    if(hand == NULL){
        return 0;
    }else if(hand->value > 10){
        return handValue(hand->nextCard) + 10;
    }else{
        return handValue(hand->nextCard) + hand->value;
    }
}

int hardValue(Card *hand){
    if(hand == NULL){
        return 0;
    }else if(hand->value > 10){
        return handValue(hand->nextCard) + 10;
    }else if(hand->value == 1){
        return handValue(hand->nextCard) + 11;
    }else{
        return handValue(hand->nextCard) + hand->value;
    }
}

int checkBust(Card *hand){
    int value = handValue(hand);

    if(value > 21){
        return 1;
    }

    return 0;
}

int checkWin(Card *playerHand, Card *compHand){
    int pValue = handValue(playerHand);
    int cValue = handValue(compHand);
    if(checkBust(playerHand) == 1){
        return -1;
    }else if(checkBust(compHand) == 1){
        return 1;
    }else if(21 - pValue > 21 - cValue){
        return 0;
    }else if(pValue == cValue){
        return 2;
    }

    return 1;
}