#include <stdio.h>
#include <cs50.h>
#include <math.h>

bool luhncheck(int ndigits, int d[]){
/* 1. */
int sum = 0;
for (int i = (ndigits - 2); i > -1; i = i - 2){
    int x = (d[i] * 2); ;
    if (x < 10)
    { sum = sum + x;}
    else
    { x = (x % 10);
        sum = sum + x + 1;}}
/* 2. */
for (int i = (ndigits - 1); i > -1;i = i - 2){
    sum = (sum + d[i]);}
/* 3. */
bool pass;
if (sum % 10 == 0)
    {pass = 1; return pass;}
else 
    {pass = 0; return pass;}} 

int main()
{
long creditn;
int ndigits;
bool luhn = 0;
/*get input (credit card number)*/
do {
creditn = get_long("Number: ");
ndigits = (floor(log10(creditn)) + 1);
if (ndigits < 13 || ndigits > 16)
    printf("INVALID\n");
}while (ndigits < 13 || ndigits > 16);
/*convert input to an array*/
int d[ndigits];
for(int i = 0; i < ndigits; i++){
   long h = pow(10, (ndigits - i));

   long l = pow(10, (ndigits - (i + 1)));
   d[i] = floor((creditn % h) / l);}
/*check if passes luhn's algorithm*/
luhn = luhncheck( ndigits, d);
if (luhn = 0)
    {printf("INVALID\n"); return 0;}
/*check and display what type of card (0=amex, 1=visa, 2=MC)*/
int cndigits[3] = { 15, 13, 16 };
char cardtype[4][11] = { "AMEX", "VISA", "MASTERCARD", "INVALID" };
int x = 3;
{for(int i = 0; i < 3; i++)
    if ((d[0] == (3 + i)) && ((ndigits == cndigits[i]) || ndigits == (3 + cndigits[i])))
        x = i;}
    
printf("%s\n", cardtype + x);
return 0;
}
