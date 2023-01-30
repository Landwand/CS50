#include <cs50.h>
#include <stdio.h>

int main(void)
{

    int start_size;
    int end_size;
  do
  {
        start_size = get_int ("What is the START SIZE? ")
        end_size = get_int ("What is the ENDING SIZE? ")


  }
  while (start_size < 9 && end_size > start_size);

    printf(start_size, end_size);

    // TODO: Calculate number of years until we reach threshold



    // TODO: Print number of years
}