#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string arg);
char rotate(char ch, int key);

int main(int argc, string argv[])
{
    if (argc != 2) // making sure that there are exactly two command-line arguments
    {
        printf("Usage: ./caesar key \n");
        return 1; //ending the run right here
    }
    bool value = only_digits(argv[1]); // checking whether the key is a digit using the only_digits function
    if (value == false)
    {
        printf("Usage: ./caesar key \n");
        return 1;
    }

    // convert the input string key into an integer
    int key = atoi(argv[1]);

    // prompt for the plain text
    string plain_text = get_string("Please provide the plain text:\n");

    // print plain text
    printf("plaintext:  %s\n", plain_text);

    //print ciphertext
    string ciphertext = plain_text;
    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        ciphertext[i] = rotate(plain_text[i], key);
    }
    printf("ciphertext: %s\n", ciphertext);
}

bool only_digits(string arg) // checking whether the key is a digit
{
    string input = arg;
    int n = strlen(arg);
    bool value = true;
    for (int i = 0; i < n; i++)
    {
        if (input[i] < 48 || input[i] > 57)
        {
            value = false;
        }
    }
    return value;
}

char rotate(char ch, int key)
{
    char ch_new;
    if (ch >= 65 && ch <= 90)
    {
        ch_new = 65 + ((ch - 65 + key) % 26);
    }
    else if (ch >= 97 && ch <= 122)
    {
        ch_new = 97 + ((ch - 97 + key) % 26);
    }
    else
    {
        ch_new = ch;
    }
    return ch_new;
}