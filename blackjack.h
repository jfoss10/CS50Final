int funds;
typedef enum Suits{HEARTS, SPADES, DIAMONDS, CLUBS}Suits;
char *suitNames[4] = {"Hearts", "Spades", "Diamonds", "Clubs"};
char suitLabels[4] = {'@','&','$','%'};
char *cardNames[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

typedef struct Card{
    int value;
    char *name;
    Suits suit;
    struct Card *nextCard;
    struct Card *prevCard;

} Card;


void greet();
int wager();
Card* createDeck();
void freeDeck();
void clear();
Card* dealCard(Card **deck);
Card* popCard(Card **deck);
void printStatus(int wager, Card *playerHand, Card *compHand, int hidden);
void printCard(Card* card);
void printHiddenCard();
void printHouseHand(Card *card);
int deckSize(Card *deck);
Card* deckToArray(Card* deck);
void moveCardToDeck(Card **card, Card **deck);
void moveHandToDeck(Card **card, Card **deck);
Card *dealHand(Card **deck);
void printHand(Card* hand, int hidden);
int requestAction();
int requestNewGame();
int handValue(Card *hand);
int softValue(Card *hand);
int hardValue(Card *hand);
int checkBust(Card *hand);
int checkWin(Card *playerHand, Card *compHand);