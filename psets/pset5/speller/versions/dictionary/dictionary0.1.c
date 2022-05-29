// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

// Number of buckets in hash table
#define N (27) // size of index (26 alpha + ') 

// Represents a node in a hash table
typedef struct node
{
    struct node *children[N];
    bool is_word;
}
node;

// Hash table
node *root;

// Load -- Sub-functions

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
        int index = 26; //value of '\''
        int a = 'a';
        if (c >= a)
            index = c - a;
        return index;
    }

// Check -- Sub-function

    // returns true if char is alpha or '\''


// Size -- Sub-definition
    int dic_size = 0; //counts number of words in dictionary;

// Unload -- Sub-function
    void free_all_core (node *n)
    {
        for (int i = 0; i < N; i++)     // for every index in node
        {
            if (n->children[i])         // if child node exists
            {
                free_all_core(n->children[i]);     // free children of child node
        }}
        free (n);                       // then free this node
    }

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int length = strlen(word);
    int index;
    char c;
    node *pCrawl = root;
    
    for (int i = 0; i < length; i++)
    {
        if (!isalpha(word[i]) && !(word[i] == ('\'')))
            return false;
        
        c = word[i];
        if (isupper(c))
            c = tolower(c);
        
        index = char_to_index(c);

        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];

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
    // Insert dictionary into hash table
    char c;
    root = alloc_node();
    node *pCrawl = root;    // set node pointer to root node
    
    while(fread(&c, sizeof(c), 1, dp) == 1)
    {
          if (c == '\n')    // end of line = end of word 
        {
            pCrawl->is_word = true;
            dic_size++;
            pCrawl = root;                      
        }
        else
        {
            int index = char_to_index(c);      // find index
            if (!pCrawl->children[index])      // if no node exists at index, create one.
                pCrawl->children[index] = alloc_node();  
            pCrawl = pCrawl->children[index];  // move pointer to next node
        }
    }
    fclose(dp);
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
    free_all_core(root); //  recurisively free all children under root node
    return true;
}
