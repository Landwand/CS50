#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"
// for NULL
#include <stddef.h>
// for FILE
#include <stdio.h>
#include <string.h>

int main (void)
{

    char * word = "Hello";
    char f_ltr = word[0];
    f_ltr = tolower(f_ltr);
    printf ("first ltr = %c \n", f_ltr);
    printf ("first ltr_int = %i \n", f_ltr);


}
