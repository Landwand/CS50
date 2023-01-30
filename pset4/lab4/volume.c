// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    // anything defined with uint8_t will have size of BYTE
    uint8_t header[HEADER_SIZE];
    // 44 bytes


    // fread(ptr to mem to store data, num_bytes_of_each_item, num_items, ptr to be read from/to )
    fread(header, HEADER_SIZE, 1, input);
    //printf("HEADER: %s\n",header);
    fwrite(header, HEADER_SIZE, 1, output);


    // TODO: Read samples from input file and write updated data to output file

    int16_t buffer;

    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        //fwrite(&buffer, sizeof(BYTE), 1, destination);
        //printf("buffer: %i\n", buffer);
        buffer = buffer * factor;
        //printf("m_buffer: %i\n", buffer);
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }


    printf("done job \n");
    // Close files
    fclose(input);
    fclose(output);
}
