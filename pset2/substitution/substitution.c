#include <cs50.h>
#include <stdio.h>
#include <string.h> // needed for strings
#include <ctype.h>


char copy_str(string);
int check_arg_count(int argc, string argv[2]);
int check_length(string key);
int check_alphabet(string key);
int check_character_freq(string key, const string alphabet);
string get_message(void);
void cipher_message(string message, string key, string ALPHABET, char coded[]);
char encode_ltr(char ltr);
string make_upper(string key);




int main(int argc, string argv[2])
{

    const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string lower_bet = "abcdefghijklmnopqrstuvwxyz";
    string r = "zyxwvutsrqponmlkjihgfedCBA";

    if (check_arg_count(argc, argv) == 0) // check for correct # of args
    {
        return 1; // if fails, MAIN fails
    }

    string key = argv[1];

    if (check_length(key) == 0) // check for exactly 26 chars in arg
    {
        printf("Error: must be exactly 26 chars long \n");
        return 1; // main program fail
    }

    if (check_alphabet(key) == 0) // check for only ALPHA characters
    {
        printf("Error: must only contain alpha characters. \n");
        return 1; // if check fails, MAIN fails
    }

    if (check_character_freq(key, ALPHABET) == 0) // check for only 1 of all characters
    {
        printf("Error: must only 1 of each alpha character. \n");
        return 1; // if check fails, MAIN fails
    }

    key = make_upper(key);

    string message = get_message();
    int l = strlen(message);
    char coded[l + 1]; //account for NULL char
    coded[l] = '\0';


    if (strlen(message) < 1) //bad message

    {
        printf("Error, bad message to decode\n");
        return 1; // MAIN fails
    }
    else
    {
        cipher_message(message, key, ALPHABET, coded);
    }

    printf("ciphertext: ");

    for (int i = 0; coded[i] != '\0'; i++)
    {
        printf("%c", coded[i]);
    }
    printf("\n");

    return 0;

} // end of MAIN



int check_arg_count(int argc, string argv[2])
{
    string cmd = argv[1];

    if (argc < 2 || argc > 3)
    {
        printf("Error: input 1 argument after program name. \n");
        return 0;
    }
    else
    {
        return 1;
    }
}




int check_alphabet(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("fn check_alpha failed. \n");
            return 0;
        }
    }

    return 1; //check alpha good.
}



int check_character_freq(string key, string ALPHABET)
{
    int char_count = 0; // init count of character instance to 0 found

    for (int i = 0, n = strlen(ALPHABET); i < n; i++)  // for each letter in ALPHA
    {

        for (int k = 0, len = strlen(key); k < n; k++) // for each in Key
        {
            key[k] = toupper(key[k]);

            if (ALPHABET[i] == key[k]) // if key-char is present:
            {
                char_count += 1;
            }

        } // end of key-for

        if (char_count > 1)
        {
            printf("char_count failed: %i \n", char_count);
        }
        else
        {
            char_count = 0;
        }
    }


    if (char_count > 1)
    {
        printf("fn chk_char_freq failed \n");
        return 0;
    }
    else
    {
        return 1;
    }

} //end check_char_freq



int check_length(string key)
{
    if (strlen(key) != 26)
    {
        return 0; // fail check
    }
    else
    {
        return 1; //successful
    }
}



void cipher_message(string message, string key, string ALPHABET, char coded[])
{

    for (int i = 0; message[i] != '\0'; i++)
    {
        char ltr = message[i]; // just easier
        int index;

        if (ltr < 123 && ltr > 96) // handle LOWER
        {
            ltr = ltr - 32; // convert to Upper

            for (int j = 0, alen = strlen(ALPHABET); j < alen; j++) // look thru ALPHA
            {
                if (ltr == ALPHABET[j])
                {
                    index = j;
                    coded[i] = key[index];
                    coded[i] = coded[i] + 32; // convert to Lower again
                }
            }
        }
        else if (ltr < 91 && ltr > 64) // handle UPPER case in message
        {
            for (int k = 0, alength = strlen(ALPHABET); k < alength; k ++) // look thru ALPHA
            {
                if (ltr == ALPHABET[k])
                {
                    index = k;
                    coded[i] = key[index];
                }
            }
        }
        else
        {
            coded[i] = ltr; //do nothing
        }

    } // end FOR

    // can also add coded[l] = '\0'; here
}





string get_message(void)
{
    string message = get_string("plaintext: ");
    return message;
}


string make_upper(string key)
{
    string upper_key = "";
    for (int i = 0, l = strlen(key); i < l; i++)   // for (int k = 0, len = strlen(key); k < n; k++)
    {
        key[i] = toupper(key[i]);
    }
    return key;
}

