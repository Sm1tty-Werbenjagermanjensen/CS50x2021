#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, char* argv[])
{/*Filter key argument and create array;how far each letter is from "A".*/
bool invalid = 0;
if(!(argc==2))
    {printf("Usage: %s key\n", argv[0]);return 1;}    
if(!(strlen(argv[1]) == 26)) 
    invalid = 1;
char k[26]; k[26] = '\0';
for(int i = 0; i < 26; i++)
    {if(!isalpha(argv[1][i]))
        invalid = 1;
    for(int j = 0; j < 26; j++)
        {if(argv[1][i] == argv[1][j] && !(i == j))
            invalid = 1;}
    if(argv[1][i] > 90)
        k[i] = argv[1][i] - 97;
    else
        k[i] = argv[1][i] - 65;}
if(invalid == 1)
    {printf("key must be 26 unique alphabetic characters\n"); return 1;}
/*Plain text input.*/
string p = get_string("plaintext: ");
int plen = strlen(p);
/*Encryption.*/
char c[plen]; c[plen] = '\0';
int x; int o;
for(int i = 0; i < plen; i++)
    {if(p[i] > 96 && p[i] < 123)
        {x = p[i] - 97; o = 97;}
    else if(p[i] > 64 && p[i] < 123)
        {x = p[i] - 65; o = 65;}
    else 
        {x = 27;c[i] = p[i];}
    if(x < 27)
        c[i] = k[x] + o;}
/*Print cipher.*/
printf("ciphertext: %s\n", c);return 0;}
