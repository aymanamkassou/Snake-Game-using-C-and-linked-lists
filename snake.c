#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 10

typedef struct node
{
    int x;
    int y;
    struct node *next;
} Snake;

int size(Snake *S)
{
    if (S == NULL)
    {
        return 0;
    }
    else
    {
        int size = 0;
        for (; S != NULL; S = S->next)
        {
            size++;
        }
        return size;
    }
}

void destroy(Snake **S)
{
    Snake *p = *S;
    Snake *temp;
    while (p != NULL)
    {
        temp = p;
        p = p->next;
        free(temp);
    }
    *S = NULL;
}

int bite(Snake *S)
{
    int xt = S->x;
    int yt = S->y;
    for (Snake *p = S->next; p != NULL; p = p->next)
    {
        if (p->x == xt && p->y == yt)
        {
            return 1;
        }
    }
    return 0;
}

Snake *insert_head(Snake *S, int x, int y)
{
    Snake *nS = (Snake *)malloc(sizeof(Snake));
    if (nS == NULL)
    {
        puts("Insufficient memory.");
        exit(1);
    }
    else
    {
        nS->x = x;
        nS->y = y;
        if (S == NULL)
        {
            nS->next = NULL;
            S = nS;
        }
        else
        {
            nS->next = S;
            S = nS;
        }
    }
    return S;
}

Snake *remove_tail(Snake *S)
{
    if (S == NULL)
    {
        puts("Nothing to remove.");
        return S;
    }
    else if (S->next == NULL)
    {
        free(S);
        return NULL;
    }
    else
    {
        Snake *p = S;
        Snake *supp;
        for (; p->next->next != NULL; p = p->next)
            ;
        supp = p->next;
        p->next = NULL;
        free(supp);
        return S;
    }
}

Snake *move(Snake *S, char direction)
{
    if (direction == 'N')
    {
        if (S->y == 0)
        {
            S = insert_head(S, S->x, 9);
            S = remove_tail(S);
        }
        else
        {
            S = insert_head(S, S->x, S->y - 1);
            S = remove_tail(S);
        }
    }
    else if (direction == 'S')
    {
        if (S->y == 9)
        {
            S = insert_head(S, S->x, 0);
            S = remove_tail(S);
        }
        else
        {
            S = insert_head(S, S->x, S->y + 1);
            S = remove_tail(S);
        }
    }
    else if (direction == 'E')
    {
        if (S->x == 9)
        {
            S = insert_head(S, 0, S->y);
            S = remove_tail(S);
        }
        else
        {
            S = insert_head(S, S->x + 1, S->y);
            S = remove_tail(S);
        }
    }
    else if (direction == 'W')
    {
        if (S->x == 0)
        {
            S = insert_head(S, 9, S->y);
            S = remove_tail(S);
        }
        else
        {
            S = insert_head(S, S->x - 1, S->y);
            S = remove_tail(S);
        }
    }
    else
    {
        puts("Invalid choice.");
    }
    if (bite(S) == 1)
    {
        printf("Lost. Snake size: %d\n", size(S));
        destroy(&S);
    }
    return S;
}

Snake *gain(Snake *S, int x1, int y1, char direction)
{
    if (S->x == x1 && S->y == y1)
    {
        if (direction == 'N')
        {
            if (S->y == 0)
                S = insert_head(S, S->x, 9);
            else
                S = insert_head(S, S->x, S->y - 1);
        }
        else if (direction == 'S')
        {
            if (S->y == 9)
                S = insert_head(S, S->x, 0);
            else
                S = insert_head(S, S->x, S->y + 1);
        }
        else if (direction == 'E')
        {
            if (S->x == 9)
                S = insert_head(S, 0, S->y);
            else
                S = insert_head(S, S->x + 1, S->y);
        }
        else if (direction == 'W')
        {
            if (S->x == 0)
                S = insert_head(S, 9, S->y);
            else
                S = insert_head(S, S->x - 1, S->y);
        }
    }
    else
    {
        S = move(S, direction);
    }

    if (bite(S) == 1)
    {
        printf("Lost. Snake size: %d\n", size(S));
        destroy(&S);
    }

    return S;
}

void display_grid(Snake *S, int xPrey, int yPrey)
{
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            if (x == xPrey && y == yPrey)
            {
                printf("P");
            }
            else
            {
                Snake *temp = S;
                int snakeFound = 0;
                while (temp != NULL)
                {
                    if (temp->x == x && temp->y == y)
                    {
                        printf("S");
                        snakeFound = 1;
                        break;
                    }
                    temp = temp->next;
                }
                if (!snakeFound)
                {
                    printf(".");
                }
            }
        }
        printf("\n");
    }
}

int main()
{
    srand(time(NULL));
    Snake *snake = insert_head(NULL, 5, 5);
    int prey_x = rand() % 10;
    int prey_y = rand() % 10;
    while (1)
    {
        display_grid(snake, prey_x, prey_y);
        printf("Enter a direction (N/S/E/W): ");
        char direction;
        scanf(" %c", &direction);
        if (snake->x == prey_x && snake->y == prey_y)
        {
            snake = gain(snake, prey_x, prey_y, direction);
            prey_x = rand() % 10;
            prey_y = rand() % 10;
        }
        else
        {
            snake = move(snake, direction);
        }
        if (bite(snake))
        {
            printf("Lost! Final snake size: %d\n", size(snake));
            destroy(&snake);
            break;
        }
    }

    return 0;
}
