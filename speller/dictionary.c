// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // ingnores case while comapring words

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
//const unsigned int N = 26;
//const unsigned int N = 676;
const unsigned int N = 2625;

// Hash table
node *table[N];

// A counter for number of elememts in the dictionary
unsigned int counter = 0;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    char tmp[LENGTH + 1];
    node *n = NULL;
    while (fscanf(file, "%s", tmp) != EOF)
    {
        counter++;
        n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n -> word, tmp);
        n -> next = NULL; // This was very important, otherwise it would cause what is called unintialized etc error with valgrind
        if (table[hash(tmp)] == NULL) // if there is nothing in the table yet
        {
            table[hash(tmp)] = n;
        }
        else
        {
            n -> next = table[hash(tmp)];
            table[hash(tmp)] = n;
        }
    }
    fclose(file); // Don't forget to close it
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int length = strlen(word);
    unsigned int hashValue;
    if (length == 1)
    {
        hashValue = (toupper(word[0]) - 'A') * 26;
    }
    else if (length == 2)
    {
        hashValue = abs((toupper(word[0]) - 'A') * 26 + (toupper(word[1]) - 'A'));
    }
    else if (length == 3)
    {
        hashValue = abs((toupper(word[0]) - 'A') * 26 + (toupper(word[1]) - 'A') * 26 + (toupper(word[2]) - 'A'));
    }
    else if (length == 4)
    {
        hashValue = abs((toupper(word[0]) - 'A') * 26 + (toupper(word[1]) - 'A') * 26 + (toupper(word[2]) - 'A') * 26 + (toupper(\
                        word[3]) - 'A'));
    }
    else
    {
        hashValue = abs((toupper(word[0]) - 'A') * 26 + (toupper(word[1]) - 'A') * 26 + (toupper(word[2]) - 'A') * 26 + (toupper(\
                        word[3]) - 'A') * 26 + (toupper(word[4]) - 'A'));
    }
    return hashValue;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return counter;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    unsigned int address = hash(word);
    node *current = table[address];
    int value;
    while (current != NULL)
    {
        value = strcasecmp(word, current -> word);
        if (value == 0)
        {
            return true;
        }
        else
        {
            current = current -> next;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor = NULL;
    node *current = NULL;
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            current = table[i];
            //while (current -> next != NULL)
            while (current != NULL)
            {
                cursor = current -> next;
                free(current);
                current = cursor;
            }
            //free(current);
        }
    }
    return true;
}
