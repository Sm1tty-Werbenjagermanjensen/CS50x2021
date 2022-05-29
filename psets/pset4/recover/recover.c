#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Define byte
typedef uint8_t byte;
// Define jpg header
byte header[4] = {0xff, 0xd8, 0xff, 0xe};
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
    while (ftell(fp) <= (file_size - 512)) // (file_size - 512) is last possible byte that can fit a block
    {
        // Make jpg name
        char jpgname[8]; 
        sprintf(jpgname, "%i%i%i.jpg", njpg/100, (njpg/10) % 10, njpg % 10); // arithmetic makes  njpg as 3 digits
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
        while (!(jpg_header(fp)) && ftell(fp) <= (file_size - 512)); /* After header, start of jpg blocks can't be 
                                                                        another header or end of file*/
        
        fclose(jpg);
        njpg++;
    }
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

bool jpg_header(FILE *fp) /* since majority of tests will be false, 
                             I tried to use this assumption to minimize lines executed */
{
    // Declare variables
    bool is_header = false;
    byte B;
    for (int i = 0; i < 3; i++)
    {
        fread(&B, 1, 1, fp);
        if (!(B == header[i]))
        {
            fseek(fp, -(i+1), SEEK_CUR);
            return is_header;    
        }
    }
    
    fread(&B, 1, 1, fp);
    B >>= 4;
    if (B == header[3])
        is_header = true;
    fseek(fp, -4, SEEK_CUR);
    return is_header;
}
