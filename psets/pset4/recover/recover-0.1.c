#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t byte;

// Function prototypes
long int get_fileSize(FILE *fp);
bool jpg_header(FILE *fp);
int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (!(argc == 2))
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    // Open file
    FILE *fp = fopen (argv[1], "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }
    // Declare variables
    int njpg = 0; // number of jpgs found
    byte buffer [512]; // 1 block = 512 bytes
    long int file_size = get_fileSize(fp);
    
    // Find 1st jpg
    while (ftell(fp) < (file_size - 4)) // (file_size - 4) is the last possible byte that can fit a header
    {
            if (jpg_header(fp)) // If current byte is 1st byte of a jpg header:
                break;          // jpg found
            fseek(fp, 1, SEEK_CUR); //Else, check the next byte
    }
    // If no jpg was found
    if (ftell(fp) == (file_size - 4))
    {
        fclose(fp);
        printf("No JPEG files found.\n");
        return 2;
    }
    // Recover jpgs
    while (njpg < 50)
    {
        // Make jpg name
        char jpgname[8]; 
        sprintf(jpgname, "%i.jpg", njpg);
        // Make jpg file
        FILE *jpg = fopen(jpgname, "w");
        if (jpg == NULL)
        {
            fclose(jpg);
            printf("Could not create %s\n", jpgname);
            return 3;          
        }
        // Write blocks to jpg
        do
        {
            fread(&buffer, 512, 1, fp);
            fwrite(&buffer, 512, 1, jpg);
        } 
        while (!(jpg_header(fp)) && ftell(fp) < file_size); //beginning of a block can't be a header or end of file
        
        fclose(jpg);
        njpg++;
    }
    printf("%i\n", ftell(fp));
    fclose(fp);
    return 0;
}  

long int get_fileSize(FILE *fp)
{
    long int cur_pos;
    long int file_size;
    cur_pos = ftell(fp); 
    fseek (fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek (fp, 0, SEEK_SET);
    return file_size;
}

bool jpg_header(FILE *fp)
{
    // Declare variables
    bool is_header = false;
    byte header[] = {0xff, 0xd8, 0xff, 0xe}; // ** last 3 bytes of header (first byte--0xff--was already tested)
    // Format sample
    byte sample[4];
    fread(&sample, 4, 1, fp);
    fseek(fp, -4, SEEK_CUR);
    sample[3] >>= 4;
    // Return false if sample value does not equal header value
    for (int i = 0; i < 4; i++)
    {
        if (!(sample[i] == header[i]))
            return is_header;
    }
    // Return true
    is_header = true;
    return is_header;
}
