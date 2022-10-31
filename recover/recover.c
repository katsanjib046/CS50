#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// declaring a new datatype
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //check whether the command-line argument is correct
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //give a name to the input file
    char *infile = argv[1];

    // open the input file
    FILE *inptr = fopen(infile, "r");

    // if the file couldn't be opened, return 1
    if (inptr == NULL)
    {
        printf("Couldn't open the file.\n");
        return 1;
    }


    //constant block size
    const int BLOCK = 512;

    //create a buffer
    BYTE buffer[BLOCK];


    // read the first four bytes
    int counter = 0;
    FILE *outptr;
    char filename[8];
    while (fread(&buffer, BLOCK, 1, inptr)) // reading data into the block
    {
        // checking the condition whther it is a jpeg. Looking just at the first four bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) // if its a jpeg
        {
            if (counter == 0)
            {
                //getting ready to make a jpeg
                sprintf(filename, "%03i.jpg", counter);
                outptr = fopen(filename, "w");
                // Create the output file
                fwrite(&buffer, BLOCK, 1, outptr);
                counter++;
            }
            else
            {
                fclose(outptr);
                //getting ready to make a jpeg
                sprintf(filename, "%03i.jpg", counter);
                outptr = fopen(filename, "w");
                // Create the output file
                fwrite(&buffer, BLOCK, 1, outptr);
                counter++;
            }
        }
        else if (counter > 0)
        {
            fwrite(&buffer, BLOCK, 1, outptr);
        }

    }

    // close the input file
    fclose(outptr); //closing the last jpeg
    fclose(inptr); // closing the input

}