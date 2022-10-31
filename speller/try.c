#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[45 + 1];
    struct node *next;
}
node;
node *table[26];
int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Couldn't open dictionary");
        return 1;
    }
    char tmp[45];
    node *n;
    while (fscanf(file, "%s", tmp) != EOF)
    {
        n = malloc(sizeof(node));
        strcpy(n -> word, tmp);
        printf("%s\n", n -> word);
        if (table[0] == NULL)
        {
            table[0] = n;
            printf("It's null\n");
        }
        else
        {
            printf("Its not null\n");
            n -> next = table[0] -> next;
            table[0] = n;
        }
        printf("%s\n", table[0] -> word);
    }
    fclose(file);
}