#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main()
{
float dollars;    
int cents,i,j,n;
int coins [] = {25, 10, 5, 1}; /*array of coin values (arranged greatest to least)*/
/*input value of change, and reprompt if less than 0 */ 
do {
    dollars = get_float("How much is owed? ");
    cents = round(dollars * 100);
}while( cents < 0);
/*determine least possible number of coins*/
for (j = 0; j < 4; j++) { 
    i = (cents / coins[j]); /*how many of a coin to return*/
    n = (n + i); /* add to total number of coins to give*/
    cents = (cents - (i * coins[j])); /*how much change is left for lesser coins*/
}

printf("%i\n", n);


return 0;
}
