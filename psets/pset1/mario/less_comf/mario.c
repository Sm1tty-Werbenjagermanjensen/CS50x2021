#include <stdio.h>
#include <cs50.h>

int main(void)
{
	
	int h;
	do
		h = get_int("How Big? : ");
	
	while(h < 1 || h > 8);
	
	printf("Height: %i\n", h);
	for(int i = 0; i < h; i++)
	{
		for (int a = h-1; a > i; a--)
		{
			printf(" ");
		}

		for (int j = -1; j < i; j++)
		{	
			printf("#");
		}

		printf("\n");
	}	

}	



