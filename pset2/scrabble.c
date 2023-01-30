#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int find_points(int letter);
int compute_score(string word);
int convert_vals(int letter);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

// Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}



int compute_score(string word) //iterates through string, adding up pts
{
    int total_score = 0;
    int length = strlen(word);

    for (int i = 0; i < length; i ++)
    {
        total_score = total_score + convert_vals(word[i]);
    }

    return total_score;
}



int convert_vals(int letter)  //finds and returns value of Letter
{
    int points = 0;

    if (letter >= 65 && letter <= 90) //Capital letter, no need to process
    {
        points = find_points(letter);
        return points;
    }
    else if (letter >= 97 && letter <= 122) // if lower case, convert to upper-case value
    {
        letter = letter - 32;  // convert to caps value
        points = find_points(letter);
        return points;
    }
    else // non alpha char, so return 0 score
    {
        points = 0;
        return points;
    }
}

int find_points(int letter) // returns point-value by looking up in Array POINTS
{
    int pos = letter - 65;  // convert to index in array; where A = 0
    int points = POINTS[pos];
    return points;
}