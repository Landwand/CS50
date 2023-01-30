#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

typedef uint8_t BYTE;

int is_header(BYTE block[]);


int main(int argc, char *argv[])
{

    // check input args for card-file-name
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // open memory card with card-file-name
    FILE *file = fopen(argv[1], "r");
    if (file != NULL)
    {
        BYTE block [512];  // use 512 bytes per block
        int jpg_count = 0;
        char jpg_name[8]; // 7 for the name, and one for NULL char
        FILE *img = NULL;

        while (fread(block, sizeof(BYTE), 512, file) > 1)
        {
            if (is_header(block) == 1) // if new JPG header found:
            {
                if (jpg_count > 0)
                {
                    fclose(img); // if file being written, close
                }

                // new file; add a name
                sprintf(jpg_name, "%03i.jpg", jpg_count);
                jpg_name[7] = '\0';
                jpg_count ++;
                img = fopen(jpg_name, "w");
            }

            // write to file
            if (jpg_count > 0)
            {
                fwrite(&block, 512, 1, img);
            }
        } // while 512 bytes loop

        // done writing files.
        fclose(img);
        fclose(file);
        return 0;
    } // file is !NULL loop

    // if file == NULL
    printf("Error with opening file; NULL. \n");
    exit(1);

} // int main


int is_header(BYTE block[])
{
    if (block[0] == 0xff &&
        block[1] == 0xd8 &&
        block[2] == 0xff &&
        (block[3] & 0xf0) == 0xe0)
    {
        // is JPG
        return 1;
    }
    return 0;
}








