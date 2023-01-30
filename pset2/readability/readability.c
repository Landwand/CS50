#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


string prompt_text(void);
string parse_each(string str);
void readability(int letters, int words, int sentence);
void print_grade(int index);



int main(void)
{
    string str = prompt_text();
    parse_each(str);
}



string prompt_text(void)
{
    string new_str;
    do
    {
        new_str = get_string("Enter string: ");
    }
    while (strlen(new_str) < 1);
    return new_str;
}


string parse_each(string str) //looks through text for letters, words, sentences & counts
{
    int letters = 0;
    int words = 0;
    int sentence = 0;

    //for(int i = 0, n = strlen(str); n < i; i++) // ask Vibhor about the ; vs ,
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        char chr = str[i];

        if (letters == 1) // count the first letter seen as a word
        {
            words++;
        }

        if ((chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122)) //check ASCII vals for all alphas
        {
            letters ++;
        }
        else if (chr == 32) // SPACE character
        {
            words ++;
        }
        else if (chr == 33 || chr == 46 || chr == 63) // check for (.) and (!)
        {
            sentence ++;
        }
    }
    readability(letters, words, sentence); // calls upon readability function
    return str;
}

void readability(int letters, int words, int sentence) // calculate 'Readability' based on Coleman-Liau index.
{
    int rounded_index;
    float L = (float)letters / words * 100;
    float S = (float)sentence / words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8; // formula
    rounded_index = roundf(index);
    print_grade(rounded_index);
}


void print_grade(index) //function to print grade results
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 1 && index <= 16)
    {
        printf("Grade %i\n", index);
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
}