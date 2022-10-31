#include <cs50.h>
#include <stdio.h>

long get_card_num(void);    // define a get_card_num function
int num_of_digits(long card_num);   // define a function to count number of digits
void validity(long card_num);   // define a luhn_algrotithm function

int main(void)
{
    long card_num = get_card_num(); // this gives card num
    int num_digits = num_of_digits(card_num);   //gives the num of digits
    validity(card_num);
}

long get_card_num(void) // function to define get the card num
{
    long card_num = 0;
    do
    {
        card_num = get_long("Please, enter your credit card number.\n"); // asking user input
    }
    while (card_num < 1);
    return card_num;
}

int num_of_digits(long card_num)   // calculated the number of digits
{
    long n = card_num;
    int count = 0;
    do
    {
        n = n / 10;
        count++;
    }
    while (n != 0);

    return count;
}

void validity(long card_num)   //the most important function to calculate the validity
{
    long num = card_num;
    int d, var; // to save the digit and a variable for luhn algo
    int sum = 0;
    int num_digits = num_of_digits(card_num);
    if (num_digits == 13 || num_digits == 15 || num_digits == 16)
    {
        for (int i = 1; i <= num_digits; i++)
        {
            d = num % 10; // store the digit at the end
            num = num / 10; // for the next loop

            if (i % 2 == 1)
            {
                sum = sum + d;
            }
            else
            {
                var = 2 * d;
                if (num_of_digits(var) == 1)
                {
                    sum = sum + var;
                }
                else
                {
                    sum = sum + (var / 10) + (var % 10) ;
                }
            }
        }
        if (sum % 10 == 0)
        {
            if (num_digits == 15 && ((int)card_num / (1e13) == 34 || (int)(card_num / (1e13)) == 37))
            {
                printf("AMEX\n");
            }
            else if (num_digits == 16 && ((int)(card_num / (1e14)) == 51 || (int)(card_num / (1e14)) == 52 || (int)(card_num / (1e14)) == 53\
                                          || (int)(card_num / (1e14)) == 54 || (int)(card_num / (1e14)) == 55))
            {
                printf("MASTERCARD\n");
            }
            else if (num_digits == 13 && (int)(card_num / (1e12)) == 4)
            {
                printf("VISA\n");
            }
            else if (num_digits == 16 && (int)(card_num / (1e15)) == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

