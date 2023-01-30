#include <cs50.h>
#include <stdio.h>
#include <string.h>


int main (void)

{

    int arr[10];
    int len = 10;
    int pos = 0;

    for (int i = 0; i < len; i++)
    {
        arr[i] = -11;
        //printf("I: %i, val: %i \n", i, arr[i]);
    }


    for (int j = 0; j < len; j++)
    {
        //printf("ARR[J++] %i \n", arr[j++]);
        arr[j++] = 00;
        //printf("J: %i, val: %i \n", j, arr[j]);
    }

    for (int k = 0; k < len; k++)
    {
        //arr[k] = -1;
        printf("k: %i, val: %i \n", k, arr[k]);
    }


}
