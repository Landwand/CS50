// Implements a dictionary's functionality

#include <ctype.h> // for tolower
#include <stdbool.h>
#include "dictionary.h"
// for NULL
#include <stddef.h>
#include <strings.h>
#include <string.h>
// for FILE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//declarations
unsigned int word_count = 0;  // keep track of the loaded Dict size

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// these are the buckets -- using a prime number; see my reference below.
const unsigned int N = 4093;

// Hash table
node *table[N];


//unsigned int malloc_count = 0; // track number of Mallocs called.
unsigned int free_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{

    unsigned int hashish = hash(word);
    node *n = table[hashish]; // look only within this hash-index
    char normalized_word [LENGTH + 1];
    strcpy(normalized_word, word);

    // consider using recursion instead of IF

    while (n != NULL)
    {

        for (int i = 0; i < strlen(word); i++)
        {
            normalized_word[i] = tolower(normalized_word[i]);
        }

        if (strcmp(n->word, normalized_word) == 0) // if they are the same
        {
            return 1;
        }

        else
        {
            n = n->next; // replace next with the current to keep iteration going
        }

    }

    return false; // word not found.
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // reference that I used to understand Hashing:  https://usaco.guide/CPH.pdf#page=255
    unsigned int hash_num = 0;
    char processed_word[LENGTH + 1];

    //hashing

    //for (int i = 0; i < (LENGTH + 1) ; i ++) //why is this 2x slower?
    for (int i = 0; i < (strlen(word)); i ++)
    {
        processed_word[i] = tolower(word[i]);
        hash_num += processed_word[i] * 3 ^ i;
    }

    hash_num = hash_num % N;  // confine to the size of the array of size ("N" - 1)

    return hash_num;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    //printf("loading!!! start! \n");
    //sleep(1);

    // for (int i = 0; i < N; i ++)
    // {
    //     table[i] = NULL;
    // }

    FILE *file = fopen(dictionary, "r");
    if (file  == NULL)
    {
        printf("cannot open DICTIONARY");
        return false;
    }

    // define word for dictionary
    char word [LENGTH + 1];

    // fscanf each word until End of File
    while (fscanf(file, "%s", word) != EOF)
    {

        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
            // no memory
        }


        unsigned int hash_val = hash(word);
        strcpy(n-> word, word);  // copy word into our node

        n->next = table[hash_val]; // set our node's (n's) "next" to point to table[hash_val] so that we don't lose it
        table[hash_val] = n;  // now set table[hash_val] "bucket" to point to our n

        word_count ++; // for use with 'size'.
    }

    fclose(file);
    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (word_count > 0)
    {
        //printf("# words loaded in DICT : %i \n", word_count);
        return word_count;
    }
    else
    {
        //printf("# 0 words loaded in DICT \n");
        return 0;
    }

}

// Unloads dictionary from memory; returns True when successful.  No reason to return False per
// https://www.reddit.com/r/cs50/comments/8ca0l1/pset5_speller_under_what_conditions_should_unload/
bool unload(void)
{


    for (int i = 0; i < N; i++) // traverse thru N values of table
    {
        node *this_node = table[i];

        while (this_node != NULL)  // node has data
        {

            node *tmp = this_node;
            this_node = this_node -> next;
            free(tmp); // free data at current location
            free_count ++;

        }

    }

    return true;
}