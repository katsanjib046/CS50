// trying to make a hash function
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
    char word[46];
    struct node *next;
}
node;

const int N = 2625;

node *table[N];
unsigned int hash(const char* word);

int main(void)
{
// ask for a word
char str[20];
printf("Enter a word\n");
scanf("%s", str);
printf("The word is %s and its hash value is %i\n", str, hash(str));
node *n = malloc(sizeof(node));
strcpy(n -> word, str);
table[hash(str)] = n;
printf("Again, the same word from table is %s\n", table[hash(str)] -> word);
free(table[hash(str)]);
}

/*unsigned int hash(char *word)
{
    return (strlen(word) == 1) ? (toupper(word[0])-'A') * 26 : (toupper(word[0])-'A') * 26 + (toupper(word[1])-'A');
}*/

unsigned int hash(const char *word)
{
    int length = strlen(word);
    unsigned int hashValue;
    if (length == 1)
    {
        hashValue = (toupper(word[0])-'A') * 26;
    }
    else if (length == 2)
    {
        hashValue = abs((toupper(word[0])-'A') * 26 + (toupper(word[1])-'A'));
    }
    else if (length == 3)
    {
        hashValue = abs((toupper(word[0])-'A') * 26 + (toupper(word[1])-'A') * 26 + (toupper(word[2])-'A'));
    }
    else if (length == 4)
    {
        hashValue = abs((toupper(word[0])-'A') * 26 + (toupper(word[1])-'A') * 26 + (toupper(word[2])-'A') * 26 + (toupper(word[3])-'A'));
    }
    else
    {
       hashValue = abs((toupper(word[0])-'A') * 26 + (toupper(word[1])-'A') * 26 + (toupper(word[2])-'A') * 26 + (toupper(word[3])-'A') * 26 + (toupper(word[3])-'A'));
    }
    return hashValue;
}