#include <cs50.h>
#include <stdio.h>
#include <string.h> // needed for strings
#include <ctype.h>



void copy_str(string orig, string dup)
{
    int length = (strlen(orig) + 1); // account for the null character
    for (int i = 0, n = strlen(orig); i < n; i ++)
        {
        dup[i] = orig[i];
        }
    //return *cpy;
}


int main (void)
{
    string hi = "Hello";
    int len = strlen(hi);
    char duplicate[(len+1)];
    string d2[]

    // for (int i = 0; i < len; i ++)
    // {
    //     copy[i] = hi[i];
    // }

    //copy_str(hi, duplicate); // puts contents of 'hi' into 'duplicate'

    copy_str (duplicate, d2);

    for (int j = 0, copylen = strlen(duplicate); j < copylen; j++)
    {
        printf("%c", duplicate[j]);
    }
    printf("\n");
}