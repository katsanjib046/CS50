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

int main(void)
{
    node *n = malloc(sizeof(node));
    strcpy(n -> word, "Sanjib");
    table[0] = n;
    if (table[0] != NULL)
    {
        n = malloc(sizeof(node));
        n -> next = table[0];
        strcpy(n -> word, "Katuwal");
        table[0] = n;
    }
    printf("%s\n",table[0] -> word);
    printf("%s\n",table[0] -> next -> word);
    free(table[0] -> next);
    free(table[0]);

}