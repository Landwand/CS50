#include <cs50.h>
#include <stdio.h>


int main(void)

{

    int dots = 0;
    int hash = 1;
    int count = 0;

    do
    {

    count = get_int("Input integer from 1-8: ");

    }
    while (count < 1 || count > 8);


        while (count >= 1)
        {
            int i;
            for (i=1; i<count; ++i)
            {
                printf(" ");
            }

            int k;
            for (k=0; k<hash; ++k)
            {
                printf("#");
            }

            printf("  ");

            int l;
            for (l=0; l<hash; ++l)
            {
                printf("#");
            }

            dots--;
            hash++;

            printf ("\n");
            count--;

        }


}