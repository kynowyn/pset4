//This program reads raw data and scans it for JPEG images.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    //Only proceed with program if usage is correct.
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover filename.raw\n");
        return 1;
    }

    //Get name of file entered.
    char *infile = argv[1];

    //File_name keeps track of how many pictures have been created so far.
    int file_name = 0;

    //Create a buffer that is 512 bytes in size.
    BYTE buffer[512];

    //Open the input file to read, and print an error if unable to do so.
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Unable to open file.\n");
        return 2;
    }

    //Read one byte at a time into the buffer from the input file.
    fread(&buffer, 512, 1, inptr);

    //Keep reading data until the buffer reaches the end of the file, at which point the buffer == 10.
    while (sizeof(buffer) == 512)
    {
        //Check if the loaded bytes signal the start of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[3] <= 0xef)
        {
            //Create a string to put the file name into.
            char outfile[17];

            //make the filename by turning the file_name integer into a three digit number.
            sprintf(outfile, "%c%c%c.jpg", (file_name / 100) + '0', (file_name / 10) + '0', (file_name % 10) + '0');

            //Open up the output file to be written using the name in the outfile string.
            FILE* outptr = fopen(outfile, "w");

            //Write the contents of the buffer to the output file.
            fwrite(&buffer, 512, 1, outptr);

            //Read the next byte of the input file.
            fread(&buffer, 512, 1, inptr);

            //Until the next file signal is reached, keep adding data into the current file.
            while (buffer[0] != 0xff || buffer[1] != 0xd8 || buffer[2] != 0xff)
            {
                 fwrite(&buffer, 512, 1, outptr);
                 fread(&buffer, 512, 1, inptr);
            }

            //Announce that the file has been created.
            printf("file %i created.\n", file_name);

            //Close the file that was written, then increment the file name by one.
            fclose(outptr);
            file_name++;

        }

        else
        {
            fread(&buffer, 512, 1, inptr);
        }
    }

    fclose(inptr);
    return 0;
}
