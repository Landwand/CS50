#include <cs50.h>
#include <stdio.h>

// for each y n/3 born, n/4 die

int pop_change(void);


int main(void)
{
    int start_size;
    int end_size;
    int population;
    int years;


  do
  {
     start_size = get_int ("What is the START SIZE? ");

  }
  while (start_size < 9);

  do
  {
    end_size = get_int ("What is the ENDING SIZE? ");
  }
  while (end_size < start_size );

  population = start_size;
  pop_change(population);
  //years +=1;

    // TODO: Calculate number of years until we reach threshold


    // TODO: Print number of years
}

int pop_change(population) {
    printf("The population is, %i/n", population)
}