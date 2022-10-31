#include <math.h>
#include <stdio.h>
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // first of all make a copy of the given image so that we use unchanged values for calculations
    RGBTRIPLE copy[height][width]; // we have padded the image surrounding by 0 to consider the corner cases
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // remember there are two kernels
    // define the first kernel
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    // define the second kernel
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // use for-loops to consider each pixel
    float redx, redy, greenx, greeny, bluex, bluey;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // set each color to zero initially
            redx = 0;
            bluex = 0;
            greenx = 0;
            redy = 0;
            bluey = 0;
            greeny = 0;
            // use for loops to consider the product of neightbors of colors of each pixel with the Kernel
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1, y = 0; l < 2; l++)
                {
                    if (i + k < 0 || j + l < 0 || i + k >= height || j + l >= width) // adjusting for corner cases
                    {
                        continue;
                    }
                    // do it for x first
                    redx += copy[i + k][j + l].rgbtRed * gx[k + 1][l + 1];
                    greenx += copy[i + k][j + l].rgbtGreen * gx[k + 1][l + 1];
                    bluex += copy[i + k][j + l].rgbtBlue * gx[k + 1][l + 1];

                    //do it for y then
                    redy += copy[i + k][j + l].rgbtRed * gy[k + 1][l + 1];
                    greeny += copy[i + k][j + l].rgbtGreen * gy[k + 1][l + 1];
                    bluey += copy[i + k][j + l].rgbtBlue * gy[k + 1][l + 1];
                }
            }
            // use sobel algorithm to combine the results of these two kernels
            image[i][j].rgbtRed = fmin(255, round(sqrt(redx * redx + redy * redy))); // x*x is not equal to x^2
            image[i][j].rgbtGreen = fmin(255, round(sqrt(greenx * greenx + greeny * greeny)));
            image[i][j].rgbtBlue = fmin(255, round(sqrt(bluex * bluex + bluey * bluey)));
        }
    }
}
