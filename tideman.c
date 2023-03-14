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
        // printf("\n                         (%s - %i, %s - %i, %s - %i)\n", candidates[0], ranks[0], candidates[1], ranks[1], candidates[2], ranks[2]); //DEBUG

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();


    /*printf("\n");
    for (int i = 0; i < candidate_count; i++) // print matrix for DEBUG
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                printf("true ");
            }
            else
            {
                printf("false ");
            }
        }
        printf("\n");
    }*/

    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i; // ranks[i] = rank
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++) //create matrix of i x j
    {
        for (int j = i + 1; j < candidate_count; j++) // was j = 0
        {
            /*if (ranks[i] < ranks[j]) iteration 1
            {
                preferences[i][j]++;
            }*/
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    /*printf("\n");
    for (int i = 0; i < candidate_count; i++) // print matrix for DEBUG
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%ii%ij%i ", preferences[i][j], i, j);
        }
        printf("\n");
    }*/
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 1; i < candidate_count; i++) //count to right side of matrix, skip i,i (always 0), then
    {
        for (int j = 0; j < i; j++) //go down until middle -- only go over each pair 1 time, since reflecting i,j - j,i has 2x pairs
        {

            if (preferences[i][j] > preferences[j][i]) //if i > j then i is winner
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                //printf("%s, %s |", candidates[pairs[pair_count].winner], candidates[pairs[pair_count].loser]);
                pair_count++; //increase pair count since pair is established
            }
            else if (preferences[i][j] < preferences[j][i])//vice versa
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                //printf("%s, %s |", candidates[pairs[pair_count].winner], candidates[pairs[pair_count].loser]);
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
    for (int i = 0; i < pair_count - 1; i++) // i count up to pair_count - 1
    {
        // j start at 0 and go up to pair count - 1 per each iteration removing first i set which are biggest to front
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            //if j victory > j+1 victory, bring j forward
            if ((preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner]) <
                (preferences[pairs[j + 1].winner][pairs[j + 1].loser] - preferences[pairs[j + 1].loser][pairs[j + 1].winner]))
            {
                pair sort = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = sort;
            }
        }
    }

    /*for (int i = 0; i < pair_count; i++)
    {
        printf("\n (%s %i, %s %i)\n", candidates[pairs[i].winner], preferences[pairs[i].winner][pairs[i].loser], candidates[pairs[i].loser], preferences[pairs[i].loser][pairs[i].winner]);
    }*/
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true; //set pairs to true in order
        for (int j = 1; j < i; j++)
        {
            if (locked[pairs[i].winner][pairs[i].loser] ==
                locked[pairs[i].loser][pairs[j].winner]) // check if locked[i][j] points true to a previously set one, change then skip if so
            {
                locked[pairs[i].winner][pairs[i].loser] = false;
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int wins;
    for (int i = 0; i < candidate_count; i++)
    {
        wins = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (!(locked[j][i]))
            {
                wins++;
            }
            if (wins == (candidate_count))
            {
                printf("%s\n", candidates[i]);
            }
        }
    }

    return;
}
