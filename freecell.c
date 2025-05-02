#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define size 52

struct Card
{
    int value;
    char suit;
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

void initDeck(card deck[])
{
    for (int i = 0; i < size; i++)
    {
        deck[i].value = i % 13 + 1;
        if (i / 13 < 1)
        {
            deck[i].suit = 'C';
        }else if (i / 13 < 2)
        {
            deck[i].suit = 'E';
        }else if(i / 13 < 3)
        {
            deck[i].suit = 'O';
        }else{
            deck[i].suit = 'P';
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
        printf("A%c ", current.suit);
    else if (current.value == 11)
        printf("J%c ", current.suit);
    else if (current.value == 12)
        printf("Q%c ", current.suit);
    else if (current.value == 13)
        printf("K%c ", current.suit);
    else
        printf("%d%c ", current.value, current.suit);
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

void userInput()
{
    int origin, destiny, opt = -1;
    do
    {
        printGame();
        do
        {
            printf("Choose one of the options bellow: \n");
            printf("0 - Exit\n");
            printf("1 - Move from a stack (1-8) to an auxiliar space (1-4)\n");
            scanf("%d", &opt);
        } while (opt != 0 && opt != 1);
        
        switch(opt)
        {
            case 0:
                return;
                break;
            case 1:
                do
                {
                    printf("Type in this format: (origin  destiny)\n");
                    scanf("%d", &origin);
                    scanf("%d", &destiny);
                }while (origin == -1 || destiny == -1 || destiny > 4 || destiny <= 0 || origin <= 0 || origin > 8);
                printf("================================================================================\n");    
                printf("ACTION: \n");
                moveToAuxiliar(origin, destiny);
                break;
        }
        printf("================================================================================\n");  
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
    userInput();
    return 0;
}