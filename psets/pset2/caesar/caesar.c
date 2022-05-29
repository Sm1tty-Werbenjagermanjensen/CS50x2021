#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, char* argv[])
{
/*obtain valid key integer*/    
if (!(argc == 2))
    {printf("Usage: %s key\n",argv[0]); return 1;}
int klen = strlen(argv[1]);
for(int i = 0; i < klen; i++)
    {char c = argv[1][i];
    if (!isdigit(c))
    {printf("Usage: %s key\n",argv[0]); return 1;}}
int k = atoi(argv[1]);
/*obtain plaintext*/
string p = get_string("plaintext: ");
int plen = strlen(p);
/*encryption*/
char c[plen]; c[plen] = '\0';
int o;
for(int i = 0; i <= plen; i++)
    {o = 0;
    if (p[i] > 96 && p[i] < 123)
        o = 97;
    if (p[i] > 64 && p[i] < 91)
        o = 65;
    if (o == 0) 
        c[i] = p[i];
    else
        c[i]=((p[i] - o) + k) % 26 + o;}
/*print ciphertext*/
printf("ciphertext: %s\n",(char*)c);
return 0;
}
