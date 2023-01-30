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
    printf("Upper key %s \n", key);


    string message = get_message();
    int l = strlen(message);
    //printf("L 57: L = %i \n", l);
    l += 1; // for NULL char
    //printf("L58: L = %i\n", l);
    //printf("L 60: str_len message = %i \n", l);
    char coded[l]; //account for NULL char
    coded[l] = '\0';
    int q = strlen(coded);
    char empty[5] = { '\0' };
    //printf("L62, L = %i \n",q);


    if (strlen(message) < 1) //bad message
    {
        printf("Error, bad message to decode\n");
        return 1; // MAIN fails
    }
    else
    {

        cipher_message(message, key, ALPHABET, coded);
        //void cipher_message(string message, string key, string ALPHABET, char coded);
    }


    for (int i = 0, n = strlen(coded); i < (n-1); i++)
    {
        //printf("L 79 length of coded %i \n", n);
        printf("%c", coded[i]);
        //printf("I is: %i \n", i);
    }
    printf("\n");

    //printf("P R O G R A M  G O O D \n");
    return 0;

} // end of MAIN



int check_arg_count(int argc, string argv[2])
{
    string cmd = argv[1];
    printf("\n");
    printf("# of args: %i\n", argc);
    printf("CMD ARG: %s\n", cmd);
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

    //   else
    //   {
    //       return 1;
    //   }
    }
    return 1; //check alpha good.
}



int check_character_freq(string key, string ALPHABET)
{
    int char_count = 0; // init count of character instance to 0 found

    for (int i = 0, n = strlen(ALPHABET); i < n; i++)  // for each letter in ALPHA
    {
        //printf("ALPHA @ : %c \n", ALPHABET[i]);

        for (int k = 0, len = strlen(key); k < n; k++) // for each in Key
        {
            key[k] = toupper(key[k]);
            //printf("key[i]: %c \n", key[i]);
            //char_count = 0;

            if (ALPHABET[i] == key[k]) // if key-char is present:
            {
                char_count += 1;
                //printf("match ALPHA: Key %c %c \n", ALPHABET[i], key[k]);
                //printf("count: %i\n", char_count);
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
        return 0; // fail check
    else
    {
        return 1;
    }
}



void cipher_message(string message, string key, string ALPHABET, char coded[])
{
    printf("this is the message at Cipher: %s\n", message);

    for (int i = 0, len = strlen(message); i < len; i++)
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
                    //printf("L204: coded[i] %c\n", coded[i]);
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
                    //printf("L216: coded[i] %c\n", coded[i]);
                }
            }
        }
        else
        {
           coded[i] = ltr; //do nothing
           //printf("L223: coded[i] %c\n", coded[i]);
        }

    } // end FOR

}





string get_message(void)
{
    string message = get_string("Put in text to cipher: ");
    return message;
}


string make_upper(string key)
{
    string upper_key ="";
    for (int i = 0, l = strlen(key); i < l; i++)   // for (int k = 0, len = strlen(key); k < n; k++)
        {
            key[i] = toupper(key[i]);
        }
    return key;
}

