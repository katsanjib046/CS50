#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helpers.h"


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //we add all the colors and its average
            BYTE color = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = color;
            image[i][j].rgbtGreen = color; // all the colors have same value now
            image[i][j].rgbtRed = color;
        }
    }

}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE blue = 0, green = 0, red = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) //this is one of the definition of sepia
        {
            blue = round(fmin(255.0, 0.131 * (image[i][j].rgbtBlue) + 0.534 * (image[i][j].rgbtGreen) + 0.272 * (image[i][j].rgbtRed)));
            green = round(fmin(255.0, 0.168 * (image[i][j].rgbtBlue) + 0.686 * (image[i][j].rgbtGreen) + 0.349 * (image[i][j].rgbtRed)));
            red = round(fmin(255.0, 0.189 * (image[i][j].rgbtBlue) + 0.769 * (image[i][j].rgbtGreen) + 0.393 * (image[i][j].rgbtRed)));
            // always remember to take a value less or equal to 255
            image[i][j].rgbtBlue = blue;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtRed = red;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][j]; // temporarily saving the pixel
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    int green, red, blue; // these were not BYTE, these were int. Why?
    float counter;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            green = 0;
            red = 0;
            blue = 0;
            counter = 0;
            for (int k = i - 1; k <= i + 1;  k++) // think about padding too, it might cause problem
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if (k < 0 || l < 0 || k >= height || l >= width) // adjusting for corner cases
                    {
                        continue;
                    }
                    counter++;
                    green = green + copy[k][l].rgbtGreen;
                    blue = blue + copy[k][l].rgbtBlue;
                    red = red + copy[k][l].rgbtRed;
                }
            }

            image[i][j].rgbtGreen = round(green / counter);
            image[i][j].rgbtBlue = round(blue / counter);
            image[i][j].rgbtRed = round(red / counter);

        }
    }
}
