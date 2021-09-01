#include <cs50.h>
#include <stdio.h>
#include <string.h>


// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j; i has an 'edge' or not.
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool already_counted(int target, int counted[]);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs; set all edges to False
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes

    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }

        }

        record_preferences(ranks);


    }



        printf("\n");


    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}



// Update ranks given a new vote ; j = rank
bool vote(int rank, string name, int ranks[])
{

    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp (name, candidates[i]) == 0) // if name = candidate_name
        {
            int ind = i;
            ranks[rank] = ind; // "rank" is "j" from Main, so ranks[rank/(j)] = ind (from CAND)
            //printf("** cand[i], ranks[rank]: %s, %i \n", candidates[i], ranks[rank]);
            return true;
        }
    }

    return false; //NO MATCH FOR ANY NAME
}



// Update preferences given one voter's ranks
void record_preferences(int ranks[]) // ranks only has INTS
{
    int counted [candidate_count]; //store candidates (as INTs) already 'counted'
    int ind = 0;

    printf(" L 138 Record Preferences *** BEGIN \n");

    // for (int k = 0; k < candidate_count; k++)
    // {
    //     printf("L 144: [k], ranks[k]: %i %i \n", k, ranks[k]);
    // }


    for (int i = 0; i < candidate_count; i ++) // each 'ranks' (INT)
    {
        int current = ranks[i];

        // go 3 layers deep, iterating through each in RANKS and then updating preferences[i][j]

        for (int j = 0; j < candidate_count; j++) // pref[j]
        {
            for (int k = 0; k < candidate_count; k++) //preferences[j][k]
            {

                //printf("L 159 IF \n");
                if (already_counted(current, counted) == true)
                {
                    //printf(" already_counted = true; skipping - \n");
                    continue;
                }
                else
                {
                    // printf("else: preferences +=1 \n");
                    // preferences[j][k] += 1; //each add means [k] is unpreferred
                    // printf("j, k, PREF[j][k] : %i %i %i\n", j, k, preferences[j][k]);
                    // counted[ind] = current;

                    if (k != i)
                    {
                        preferences[j][k] += 1; //each add means [k] is unpreferred
                        //printf("j, k, PREF[j][k] : %i %i %i\n", j, k, preferences[j][k]);

                        for (int l = 0; l < candidate_count; l++)
                        {
                            printf(" J: %i \n", j);
                            printf(" L: %i \n", l);
                            printf(" PREF[j][l] %i\n", preferences[j][l]);
                        }


                        //counted[ind] = current;
                    }
                    counted[ind] = current;


                }
                printf("done for K loop \n");
            }

        }

    }

    return;
}



// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

bool already_counted(int target, int counted[])
{
    for (int i = 0; i < candidate_count; i ++)
    {
        if (target == counted[i])
        return true;
    }

    //else
    return false;
}
