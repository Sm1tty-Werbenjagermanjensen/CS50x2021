#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int start_size = 0;
    while (start_size < 9)
       {start_size = get_int("Start size: ");}

    // Prompt for end size
    int end_size = 0;
    while (end_size < start_size)        
        {end_size = get_int("End size: ");}

    int y = 0; // Number of years

    // Calculate number of years until we reach threshold
    while(start_size < end_size)
    {
        start_size = start_size + (start_size / 3) - (start_size / 4);
        y++;
    }
    // Print number of years
        printf("Years: %d\n", y);
}           
