#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
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
bool cycle(int start, int end);

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

    // Clear graph of locked in pairs
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

        record_preferences(ranks); //for each voter, inside the loop

        printf("\n");
    }
    /*  printing the preferences
        printf("Printing the Preferences\n");
        for (int k = 0; k < candidate_count; k++)
        {
            for (int l = 0; l < candidate_count; l++)
        {
            printf("%i    ", preferences[k][l]);
        }
        printf("\n");
        }
        printf("\n");

        // printing the preferences */

    add_pairs();
    /*  // Printing the pairs
        printf("Printing the Pairs\n");
        for (int k = 0; k < pair_count; k++)
        {
            printf("Pair %i: (%i %i) and value: %i\n", k, pairs[k].winner, pairs[k].loser, preferences[pairs[k].winner][pairs[k].loser]);
        }
        //printing the pairs */

    sort_pairs();

    /*  // Printing the Sorted pairs
        printf("Printing the sorted Pairs\n");
        for (int k = 0; k < pair_count; k++)
        {
            printf("Pair %i: (%i %i) and value: %i\n", k, pairs[k].winner, pairs[k].loser, preferences[pairs[k].winner][pairs[k].loser]);
        }
        //printing the Sorted pairs */

    lock_pairs();
    /*  // printing the adjacency matrix
        printf("Printing the adjacency matrix\n");
        for (int k = 0; k < candidate_count; k++)
        {
            for (int l = 0; l < candidate_count; l++)
            {
                printf("%i  ",locked[k][l]);
            }
            printf("\n");
        }
        //printing the adjacency matrix */
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0) //assuming that names don't repeat
        {
            ranks[rank] = i; //rank "rank" belongs to the ith person, whose name matched
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++; //adding 1 to the matrix everytime, one candidate is preferred over the other
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Lets sort using selection sort
    for (int i = 0; i < pair_count - 1; i++)
    {
        int hold = i; // just to hold the index of smallest temporarily
        pair temp;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[hold].winner][pairs[hold].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                hold = j;
            }
        }
        temp = pairs[hold];
        pairs[hold] = pairs[i];
        pairs[i] = temp;
    }
    return;
}


// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int count = 0; count < pair_count; count++)
    {
        if (!cycle(pairs[count].loser, pairs[count].winner))
        {
            locked[pairs[count].winner][pairs[count].loser] = true;
        }
    }
    return;
}

// check whether start and end points create a cycle
bool cycle(int loser, int winner)
{
    if (loser == winner)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner])
        {
            return cycle(loser, i);
        }
    }
    return false;
}

// winner has all the arrow going and not coming to them
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int fcount = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (!locked[j][i])
            {
                fcount++;
                if (fcount == candidate_count)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
    return;
}