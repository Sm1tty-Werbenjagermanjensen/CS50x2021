// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

#include "dictionary.h"

// Number of buckets in hash table
#define N (27) // size of index (26 alpha + ') 
#define APOSTROPHE ('\'')

// Represents a node in a hash table
typedef struct node
{
    struct node *children[N];
    bool is_word;
}
node;

// Hash table
node *root;

//Load() SubFunction Prototypes
static inline node *alloc_node(void);
static inline int char_to_index(char c);
static inline off_t file_size(const char *fname);

// Global variables
// Size
    int dic_size = 0; //counts number of words in dictionary;
// Unload -- Sub-function + defintion
    node *alloced[367087];  // array of pointers to every node allocated (367087 = # of allocs for large dic)                            
    int nalloc = 0; // # of nodes allocated  

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int i = 0;
    int index;
    node *pCrawl = root;
    char c;
    
    while(!(word[i] == '\0'))
    {  
       
        c = word[i];
        // if letter is uppercase, make lowercase.
        if (isupper(c))
            c = tolower(c);
        // create index.
        index = char_to_index(c);
        // if no child exists at index, word isn't in table.
        if (!pCrawl->children[index])
            return false;
        // set pCrawl to child node, and incriment "i"
        pCrawl = pCrawl->children[index];
        i++;

    }
    return (pCrawl->is_word);
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // char_to_index can simply be reused, rendering this function useless for this implementation.
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Create pointer for dictionary file
    FILE *dp = fopen(dictionary, "r");
    if (dp == NULL)
    {
        return false;
    }
    //read dictionary to a variable
    off_t fsize = file_size(dictionary);
    int nChars = fsize / sizeof(char);
    char Dic[nChars];
    fread(&Dic, fsize, 1, dp);
    fclose(dp);
    // Insert dictionary into hash table
    root = alloc_node();
    node *pCrawl = root;    // set node pointer to root node
       for (int i = 0; i < nChars; i++)
    {
          if (Dic[i] == '\n')    // end of line = end of word 
        {
            pCrawl->is_word = true;
            dic_size++;
            pCrawl = root;                      
        }
        else
        {
            int index = char_to_index(Dic[i]);      // find index
            if (!pCrawl->children[index])      // if no node exists at index, create one.
            {
                pCrawl->children[index] = alloc_node();
                alloced[nalloc] = pCrawl->children[index];
                nalloc++;
            }
            pCrawl = pCrawl->children[index];  // move pointer to next node
        }
    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dic_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
   while (nalloc <= 0)
   {
       free(alloced[nalloc]);
       nalloc--;
   }
    return true;
}


// Load() SubFunctions:

    // Create new node
    static inline node *alloc_node(void)
    {
        node *new_node = calloc(1, sizeof(node));
        if (new_node == NULL)
        {
            printf("Failed to allocate memory for new node.\n");
            exit(1);
        }
        return new_node;
    }

    // Convert character to index value
    static inline int char_to_index(char c)
    {
        int index = 26; //index of APOSTROPHE
        int a = 'a';
        if (!(c == APOSTROPHE))
            index = c - a;
        return index;
    }

    // Find File Size  **returns to beginning of file**
    static inline off_t file_size(const char *fname)
    {
        struct stat info;
        if (stat(fname, &info) == 0)
            return info.st_size;
        return 0;
    }
