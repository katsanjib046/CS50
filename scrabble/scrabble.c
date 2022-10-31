#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);
string convert_to_lower(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    // printf("%i\n", score1);
    int score2 = compute_score(word2);
    // printf("%i\n", score2);

    // TODO: Print the winner
    if (score1 > score2) //comparing the scores
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else // case of a tie
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int score = 0;
    int n = strlen(word);
    string lower_word = convert_to_lower(word);
    for (int i = 0; i < n; i++)
    {
        if (lower_word[i] >= 97 && lower_word[i] <= 122)
        {
            score = score + POINTS[(lower_word[i] - 97)]; // because POINTS[0] is a and so on
        }
    }
    return score;
}

string convert_to_lower(string word)
{
    // this function changes all the letters to lower cases
    char ch;
    int n = strlen(word);
    string lower_word = word; // making a string of equal length as input word
    for (int i = 0; i < n; i++) // loop over the input_word
    {
        ch = tolower(word[i]); // using tolower function
        lower_word[i] = ch;
    }
    return lower_word;
}