#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;
    do
    {
        height = get_int("Please choose a number between 1 and 8 (inclusive).\n");
    }
    while (height < 1 || height > 8); // Ask for an integer until you receive a valid input

    for (int i = 1; i <= height; i++) // loop for the heights
    {
        for (int k = height - i; k > 0 ; k--) //loop for each row to make spaces
        {
            printf(" ");
        }
        for (int j = 1; j <= i; j++) // loop for number of #
        {
            printf("#");
        }
        printf("  ");
        for (int j = 1; j <= i; j++) // loop for number of #
        {
            printf("#");
        }
        printf("\n"); // change the rows, go to next line
    }
}