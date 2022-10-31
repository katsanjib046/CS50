#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float calculate_index(int num_of_letters, int num_of_words, int num_of_sentences);
void find_grade(float index);

int main(void)
{
    string text = get_string("Text: ");
    int num_of_letters = count_letters(text);
    int num_of_words = count_words(text);
    int num_of_sentences = count_sentences(text);
    float index = calculate_index(num_of_letters, num_of_words, num_of_sentences);
    find_grade(index);
}

int count_letters(string text)
{
    // this function counts all the alphabetical characters in the text
    int num_of_letters = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122)) // you can use isalpha from ctype.h
        {
            num_of_letters ++;
        }
    }
    return num_of_letters;
}

int count_words(string text)
{
    // This is a function to count words in the text. Words end by a space in between.
    int num_of_words = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        if (isspace(text[i])) // we have used isspace instead of doing text[i] == 32
        {
            num_of_words ++;
        }
    }
    return num_of_words + 1; // adding one because the last word doesn't end with a space
}

int count_sentences(string text)
{
    // This function counts the number of sentences, which means, it counts the total number of . or ? or !.
    int num_of_sentences = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63) // we have used isspace instead of doing text[i] == 32
        {
            num_of_sentences++;
        }
    }
    return num_of_sentences;
}

float calculate_index(int num_of_letters, int num_of_words, int num_of_sentences)
{
    float L = 100.0 * num_of_letters / num_of_words; // letters per 100 words
    float S = 100.0 * num_of_sentences / num_of_words; // sentences per 100 words
    float index = 0.0588 * L - 0.296 * S - 15.8;
    return index;
}

void find_grade(float index)
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        int grade = round(index);
        printf("Grade %i\n", grade);
    }
}