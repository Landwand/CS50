#include <cs50.h>
#include <stdio.h>

int checksum(int total, int valid);
void find_card_type(int card_num);
long ask_for_card(void);

int main(void)

{
    int count = 1;
    int mod = 0;
    int sum = 0; // the sum of digits that are not multipled
    int mult;
    int first_two;
    int mult_sum; // sum of the digits that are to be multiplied (*)
    int total;
    int valid = 0; // stores whether the checksum worked for 1 or 0
    long card = ask_for_card();


    while (card > 1)
    {
        if (card < 60 && card > 10)
        {
            first_two = card;
        }

        mod = card % 10;

        if (count % 2 == 0) // if the processing digit is Even . . .
        {
            if (mod * 2 > 9) // if modulo * 2 > 10, sum the digits example : 11 = 1 + 1
            {
                mult = mult + ((mod * 2) % 10) + 1;  // add this to the mult_total

            }
            else
            {
                mult = mult + (mod * 2);  // if single digit, just add it to 'mult'
            }

        }
        else // for the odd digits, just add them up
        {
            sum = sum + mod;
        }

        count ++;
        card = card / 10 ;
        total = sum + mult;
    }   //end of while


    if (checksum(total, valid) == 1)
    {
        find_card_type(first_two);
    }
    else
    {
        printf("INVALID\n");
    }


}  // end of Main



long ask_for_card(void)
{
    long card;
    card = get_long("Input the card : ");

    if (card > 1000000000000 && card < 9999999999999999) // check for valid number of digits for input card
    {
        return card;
    }
    else
    {
        card = 0;
    }
    return card;
}



int checksum(int total, int valid) // Lunh's Algorithm.
{
    int result;

    if (total % 10 == 0)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}



void find_card_type(int first_two)
{
    // AMEX 34 / 37.
    // MC = 51, 52, 53, 54, or 55
    // Visa = 4xx

    if (first_two > 10)
    {
        if (first_two / 10 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            switch (first_two) //testing for other card types
            {
                case 34:
                case 37:
                    printf("AMEX\n");
                    break;
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                    printf("MASTERCARD\n");
                    break;
                default:
                    printf("INVALID\n");
            }
        }
    }


} //end of find_card_type