#include <stdio.h>
#include <cs50.h>

int main(void)
{
	
	int h;
	do
		h = get_int("How Big? : ");
	
	while(h < 1 || h > 8);
	
	printf("Height: %i\n", h);
}	



