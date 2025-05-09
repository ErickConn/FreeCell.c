#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define size 52

struct suit
{
    char suit;
    int suitNum;
}; typedef struct suit suit;

struct Card
{
    int value;
    suit suit;
}; typedef struct Card card;

struct no
{
    card card;
    struct no *next;
}; typedef struct no *noPtr;

card deck[52];
// Static list, each position is a stack.
noPtr stack[8];
card auxList[4];
card suitList[4];

void pushToStack(card newCard, noPtr *stack)
{
    noPtr p = (noPtr)malloc(sizeof(struct no));
    p->card = newCard;
    p->next = *stack;
    *stack = p;
}

void popOfStack(noPtr *stack)
{
    noPtr p = *stack;
    p = *stack;
    *stack = (*stack)->next;
    free(p);
}

void initializeStack(noPtr stack[])
{
    for (int i = 0; i < 8; i++) {
        stack[i] = NULL;
    }
}

void moveToAuxiliar(int origin, int destiny)
{
    card aux;
    if (auxList[destiny - 1].value != 0)
    {
        printf("Auxiliar list in position %d is already full or doesn't exist!\n", destiny);
        return;
    }
    aux = stack[origin - 1]->card;
    popOfStack(&stack[origin - 1]);
    auxList[destiny - 1] = aux;
    printf("Moved from the stack %d to auxiliar %d!\n", origin, destiny);
}

int verifyMove(card cardO, card stack)
{
    // We need to verify the suit, and the number of the card on top of the stack
    if((cardO.suit.suitNum % 2 == 0 && stack.suit.suit % 2 == 0) || (cardO.suit.suitNum % 2 != 0 && stack.suit.suit % 2 != 0))
        return 1;
    printf("Move can't be done because of the incompatibility of the suits!\n");
    return 0;
}

void moveFromAuxiliar(int origin, int destiny)
{
    card aux;
    if(auxList[origin - 1].value == 0)
    {
        printf("There is no card on this auxiliar position!\n");
        return;
    }
    aux = auxList[origin - 1];
    if (verifyMove(aux, stack[destiny - 1]->card))
    {
        auxList[origin - 1].value = 0;
        auxList[origin - 1].suit.suit = 0;
        auxList[origin - 1].suit.suitNum = 0;
        pushToStack(aux, &stack[destiny - 1]);
        printf("Moved from the auxiliar %d to stack %d!\n", origin, destiny);
    }
}

void moveBetweenStacks(int origin, int destiny)
{
    card aux;
    if(verifyMove(stack[origin - 1]->card, stack[destiny - 1]->card))
    {
        aux = stack[origin - 1]->card;
        popOfStack(&stack[origin - 1]);
        pushToStack(aux, &stack[destiny - 1]);
        printf("Moved from the stack %d to stack %d!\n", origin, destiny);
    }

}

void initDeck(card deck[])
{
    for (int i = 0; i < size; i++)
    {
        deck[i].value = i % 13 + 1;
        if (i / 13 < 1)
        {
            deck[i].suit.suit = 'C';
            deck[i].suit.suitNum = 0;
        }else if (i / 13 < 2)
        {
            deck[i].suit.suit = 'E';
            deck[i].suit.suitNum = 1;
        }else if(i / 13 < 3)
        {
            deck[i].suit.suit = 'O';
            deck[i].suit.suitNum = 2;
        }else{
            deck[i].suit.suit = 'P';
            deck[i].suit.suitNum = 3;
        }
    }
}

void shuffleCards(card deck[])
{
    card aux;
    int j;
    for(int i = size - 1; i > 0; i--)
    {
        // Generates random number from 0 to N
        j = rand() % (i+1);
        aux = deck[i];
        deck[i] = deck[j];
        deck[j] = aux;
    }
}

// We need to divide the 52 cards into 8 stacks
void divideInStacks(card deck[], noPtr stack[])
{
    int stackIndex;
    for(int i = 0; i < size; i++)
    {
        stackIndex = i % 8;
        pushToStack(deck[i], &stack[stackIndex]);
    }
}

void convertCard(card current)
{
    if (current.value == 1)
        printf("A%c ", current.suit.suit);
    else if (current.value == 11)
        printf("J%c ", current.suit.suit);
    else if (current.value == 12)
        printf("Q%c ", current.suit.suit);
    else if (current.value == 13)
        printf("K%c ", current.suit.suit);
    else
        printf("%d%c ", current.value, current.suit.suit);
}

void printGame()
{
    printf("\tAuxiliar stacks:\t\t\t\tSuits stacks:\n");
    for (int j = 0; j < 4; j++)
    {
        if (auxList[j].value == 0)
            printf("[   ]\t");
        else
        {
            printf("[ ");
            convertCard(auxList[j]);
            printf("]\t");
        }
    }
    printf("\t\t[   ]\t");
    for (int k = 0; k < 3; k++)
    {
        printf("[   ]\t");
    }
    printf("\n");
    printf("================================================================================\n");
    printf("\n");
    printf("Top ---------------------------- Bottom\n");
    for (int i = 0; i < 8; i++)
    {
        printf("Stack %d: ", i + 1);
        noPtr current = stack[i];
        while (current != NULL)
        {
            convertCard(current->card);
            current = current->next;
        }
        printf("\n");
    }
    printf("\n");
    printf("================================================================================\n");
}

void inputHandler(int opt)
{
    int origin, destiny = -1;
    if (opt == 0)
        {
            return;
        }else
        {
            do
            {
                printf("Type in this format: (origin  destiny)\n");
                scanf("%d", &origin);
                scanf("%d", &destiny);
            }while (origin == -1 || destiny == -1);
            printf("================================================================================\n");    
            printf("ACTION: \n");
            if (opt == 1)
                moveToAuxiliar(origin, destiny);
            if (opt == 2)
                moveFromAuxiliar(origin, destiny);
            if (opt == 3)
                moveBetweenStacks(origin, destiny);
        }
        printf("================================================================================\n");  
}

void userMenu()
{
    int opt = -1;
    do
    {
        printGame();
        do
        {
            printf("Choose one of the options bellow: \n");
            printf("0 - Exit\n");
            printf("1 - Move from a stack (1-8) to an auxiliar space (1-4)\n");
            printf("2 - Move from an auxiliar space (1-4) to a stack (1-8)\n");
            printf("3 - Move from a stack (1-8) to a stack (1-8)\n");
            scanf("%d", &opt);
        } while (opt < 0 && opt > 2);

        inputHandler(opt);
        
    }while(opt != 0);
}


int main()
{
    srand(time(NULL));
    initializeStack(stack);
    initDeck(deck);
    shuffleCards(deck);
    divideInStacks(deck, stack);
    printf("================================================================================\n");    
    printf("\t\t\tWelcome to the freecell game!\n");
    printf("================================================================================\n");   
    userMenu();
    return 0;
}