#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

// Prototypes
bool end_word(char d, char c, char b);
bool end_sentence(char c, char b);
int Coleman_Liau_Index(float L, float S);

int main()
{
    char* sample = get_string("Text: ");
    int nChars = strlen(sample);
    int nLetters = 0;
    int nWords = 0;
    int nSentences = 0;

    for (int i = 0; i < nChars; i++)
    {
        if (isalpha(sample[i]))    // letter is a-z or A-Z
            nLetters++;
        if  (i > 0)
        {
            if (end_word(sample[i+1], sample[i], sample[i-1]))
                nWords++;
            if (end_sentence(sample[i], sample[i-1]))
                nSentences++;
        }
    }
    // Calculate L and S variables in Coleman-Liau index
    float L = 100.0 * nLetters / nWords;     // 100.0 is needed to make numerator a float instead of int (int would truncate)
    float S = 100.0 * nSentences / nWords;
    // Calculate grade level of text
    int index = Coleman_Liau_Index(L, S);
    // Print grade level
    if (index > 16)
        printf("Grade 16+\n");
    else if (index < 1)
        printf("Before Grade 1\n");
    else
        printf("Grade %i\n", index);
    return 0;
}

bool end_word(char d, char c, char b)
{
    if (c == ' ' || d == '\0')  // if current char is a space or end of string:
        if (!(b == ' '))        // and previous char is not already a space:
            return true;        // count a word
    return false;
}

bool end_sentence(char c, char b)
{
    if ( c == '.' || c == '!' || c == '?')              // if current char is ending punctuation
        if (!(b == '.') && !(b == '!') && !(b == '?'))  // and previous char is not:
            return true;                                // count a sentence
    return false;
}

int Coleman_Liau_Index(float L, float S)
{
    int index;
    index = round(0.0588 * L - 0.296 * S - 15.8);
    return index;
}
