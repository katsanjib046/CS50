#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool only_26char(string arg); //checks if there are 26 alphabetical characters
bool non_repitition(string arg); //checks if alphabets are not repeated in the key
string cipher(string text, string key); // the function to cipher the input text

int main(int argc, string argv[])
{
    // check that that the number of command-line arguments is correct, that is, two
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // check whether the key is an integer
    bool value = only_26char(argv[1]);
    if (value == false)
    {
        printf("Key must contain 26 characters. \n");
        return 1;
    }
    //check whether there is any repitition in the key
    bool non_rep = non_repitition(argv[1]);
    if (non_rep == false)
    {
        printf("Key must not have repitition. \n");
        return 1;
    }
    string plain_text = get_string("plaintext:  ");
    string cipher_text = cipher(plain_text, argv[1]);
    printf("ciphertext: %s\n", cipher_text);
}

bool only_26char(string arg) // checking whether the key is 26 in length and contains all the characters
{
    string input = arg;
    int n = strlen(arg);
    bool value = true;
    string inputc = arg; //copy of input
    if (n != 26)
    {
        value = false;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if ((input[i] < 65 || input[i] > 90) && (input[i] < 97 || input[i] > 122)) //checking for non-alphabets
            {
                value = false;
                break;
            }
        }
    }
    return value;
}

bool non_repitition(string arg)
{
    // function to check whether all the characters are unique in the input key
    bool value = true;
    string arg_copy = arg;
    int n = strlen(arg);
    int count = 0;
    while (count < n)
    {
        arg_copy[count] = tolower(arg_copy[count]); // this will change the key to lower
        count++;
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if ((i != j) && (arg_copy[i] == arg_copy[j])) //checking for repitition
            {
                value = false;
                break;
            }
        }
    }
    return value;
}

string cipher(string text, string key)
{
    // given the text and key,this function outputs the cipher text
    int n = strlen(text); // length of the text
    string cipher_text = text;
    char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                         'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
                        };
    int i = 0;
    while (i < n)
    {
        if (text[i] <= 122 && text[i] >= 97) // for lower case
        {
            int count = 0;
            while (count < 26)
            {
                if (text[i] == alphabet[count])
                {
                    cipher_text[i] = key[count];
                    break;
                }
                count++;
            }
        }
        else if (text[i] >= 65 && text[i] <= 90) // for upper case
        {
            int count = 0;
            while (count < 26)
            {
                if (text[i] + 32 == alphabet[count])
                {
                    cipher_text[i] = key[count] - 32;
                    break;
                }
                count++;
            }
        }
        i++;
    }
    return cipher_text;
}