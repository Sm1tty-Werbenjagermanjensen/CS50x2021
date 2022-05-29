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
    uint8_t *header = malloc( HEADER_SIZE * sizeof(uint8_t)); 
    fread (header, sizeof(uint8_t), HEADER_SIZE, input);
    fwrite (header, sizeof(uint8_t), HEADER_SIZE, output);
    // TODO: Read samples from input file and write updated data to output file
        // Find number of samples
        fseek (input, 0, SEEK_END);
        long int input_size = ftell(input);
        fseek (input, HEADER_SIZE * sizeof(uint8_t), SEEK_SET);
        long int nsamples = (input_size - (HEADER_SIZE * sizeof(uint8_t))) / sizeof(int16_t);
        free (header);
        // Read input samples
        int16_t *samples = malloc(nsamples * sizeof(int16_t));
        fread (samples, sizeof(int16_t), nsamples, input);
        // Apply volume factor to samples
        if (!(factor == 1)) // no point in running loop if factor is 1
            for (int i = 0; i < nsamples; i++)
                samples[i] = samples[i] * factor;
        // write modified samples to output
        fwrite (samples, sizeof(int16_t), nsamples, output); 
        free (samples);

    // Close files
    fclose(input);
    fclose(output);
}
