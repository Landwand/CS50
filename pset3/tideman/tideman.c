#include <cs50.h>
#include <stdio.h>
#include <string.h>


// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] == true ; "i" has an EDGE over "j"
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
bool cycle(int loser, int winner);
bool has_cycle2(int winner, int loser);
void print_preferences(void);

bool has_cycle2(int winner, int loser);
//bool has_cycle3 (int winner, int loser) bool
bool has_cycle4(int winner, int loser);


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
        // ranks[i] is voter's  i'th preference
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
    } //for loop; voter_count

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();

    return 0;
}


// Update RANKS given a new vote ; j = rank
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            int ind = i;
            ranks[rank] = ind; // "rank" is "j" from Main, so ranks[rank/(j)] = ind (from CAND)
            return true;
        }
    }
    return false; // NO MATCH FOR ANY NAME
}


// Update 'preferences' given one voter's ranks
void record_preferences(int ranks[])  // RANKS[int]
{

    for (int i = 0; i < candidate_count; i++)
    {

        // i+1 is used to skip-over counting a CAND against itself when
        // adding to matrix
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]] [ranks[j]]++;
            // ranks[i/j] is used instead of the iterator "i" or "j"
            // ; this gives us the Candidate as an "int"
            // eg. candidates[3] vs i = 0
        }
    }
    return;
}


// record pairs of candidates into preferences[preferred][not_preferred]
void add_pairs(void)
{

    for (int i = 0; i < candidate_count; i++)
    {
        // i + 1 prevents needless re-checking of a candidate to itself
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair p;
                p.winner = i;
                p.loser = j;
                // increment AND add all at once!
                pairs[pair_count++] = p;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pair p;
                p.winner = j;
                p.loser = i;
                // increment AND add all at once!
                pairs[pair_count++] = p;
            }
        }
    }

    return;
}


// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // bubble sort used, largest vals to left
    int swap = -1;
    for (int i = 0, j = i + 1; i < pair_count - 1; i++)
    {
        if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[i].winner][pairs[i].loser])
        {
            pair temp = pairs[i];
            pairs[i] = pairs[j];
            pairs[j] = temp;
            swap = 1;
        }
    }
    return;
}


// this is the only 'has_cycle' function that works
bool has_cycle2(int winner, int loser)
{
    if (winner == loser)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (has_cycle2(winner, i))
            {
                return true;
            }
        }

    }

    return false;
}


bool has_cycle3(int winner, int loser)
/* NOT WORKING -- uses an IF statement instead in recursive call */
{
    if (winner == loser)
    {
        //cycle detected
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {

        if (locked[loser][i])
        {
            if (has_cycle3(i, winner)) // flipping the i & winner order -- this causes it to fail -- why?
            {
                return true;
            }
        }
    }
    return false;
}


bool has_cycle4(int winner, int loser)
/* NOT WORKING -- */
{
    if (winner == loser)
    {
        //cycle detected
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {

        if (locked[loser][i])
        {
            // notice the lack of a nested IF statement when calling itself - this causes it to
            // not-work.  Why?
            return (has_cycle4(winner, i));
        }
    }
    return false;
}



// bool has_cycle (int winner, int loser)
// /* my code */
//     {
//         if (winner == loser)
//         {
//             //cycle detected
//             return true;
//         }

//         for (int i = 0; i < candidate_count; i++)
//         {

//             if (locked[loser][i])
//              {
//                 //return (has_cycle(i, loser)); //not working, more success than (has_cycle(i,winner)

//                 // this block works
//                 if(has_cycle(i, winner))  // works 100%
//                 {
//                     return true;
//                 }
//                 //end block
//               }

//             // if (locked[loser][i]) // doesn't work
//             // {
//             //     //return (has_cycle(i, winner));   // not working

//             //     if(has_cycle(winner, i))
//             //     {
//             //         return true;
//             //     }
//             // }


//             // if (locked[winner][i]) // this works!
//             // {
//             //     return (has_cycle(i, loser));   // not working

//             //     // if(has_cycle(i, loser))
//             //     // {
//             //     //     return true;
//             //     // }
//             // }



//         } //for loop
//         return false;
//     }


/* Lock pairs into the candidate graph in order, without creating cycles */
// this function calls has_cycle2 ; the only one that works
void lock_pairs(void) //my code
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!has_cycle2(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}


// Print the winner of the election
void print_winner(void)
{
    // iterate of COL vs ROW this time!
    // We need to find CAND who doesn't have any Edge over them
    for (int col = 0; col < candidate_count; col ++)
    {
        bool found_source = true; // assume this is true until otherwise proven
        for (int row = 0; row < candidate_count; row ++)
        {
            if (locked[row][col] == true)
            {
                found_source = false;
                break; // no need to to keep looking as soon as we find 1 edge
            }
        }
        if (found_source == true)
        {
            printf("%s\n", candidates[col]);
            return;
        }
    }
    return;
}


void print_preferences(void)
{
    for (int j = 0; j < candidate_count; j++)
    {
        if (j == 0)
        {
            printf("a \n");
        }
        if (j == 1)
        {
            printf("b \n");
        }
        if (j == 2)
        {
            printf("c \n");
        }
        printf(" preferences[j] %i %i %i\n", preferences[j][0], preferences[j][1], preferences[j][2]);
    }
}