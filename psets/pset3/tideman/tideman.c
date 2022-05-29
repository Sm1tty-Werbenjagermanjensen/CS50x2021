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
void merge_sort(pair pairs[], int l, int r);
void merge(pair pairs[], int l, int m, int r);
void lock_pairs(void);
bool cycle_check(int W[], int L[]);
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

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //clear ranks upon new voter
    if (rank == 0)
        for (int i = 0; i < candidate_count; i++)
            ranks[i] = -1; // -1 = null value
    // check vote for name of valid candidate
    int candidate = -1;
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            candidate = i;
            break;
        }
    }
    if (candidate == -1)
        return false;
    // check for double voting
    for (int i = 0; i < candidate_count; i++)
    {
        if (ranks[i] == candidate && !(i == rank))
            return false;
    }
    // write vote
    ranks[rank] = candidate;
    return true;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j) 
                preferences[ranks[i]][ranks[j]]++;
        }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    /* if votes for candadite i over j are greater than j over i,
       add pair and make i the winner*/
    for (int i = 0; i < candidate_count; i++)
        for (int j = 0; j < candidate_count; j++)
            if (preferences[i][j] > preferences[j][i])
            {
                pair_count++;
                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1].loser = j;
            }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    merge_sort(pairs, 0, pair_count - 1);
    return;
}
//merge sort algorithim: l = left index, r = right index
void merge_sort(pair pairs[], int l, int r)
{
    if (l < r)
    {
        //m = middle index
        int m = (l + r) / 2;

        //sort left, then sort right
        merge_sort(pairs, l, m);
        merge_sort(pairs, m + 1, r);
        //merge
        merge(pairs, l, m, r);
    }
}

//merge subarrays of merge_sort
void merge(pair pairs[], int l, int m, int r)
{
    int i, j, k;
    //size of l and r temp arrays
    int nl = m - l + 1;
    int nr = r - m;
    //create l and r temp arrays
    pair L[nl], R[nr];
    //copy corresponding data to temp arrays
    for (i = 0; i < nl; i++)
        L[i] = pairs[l + i];
    for (j = 0; j < nr; j++)
        R[j] = pairs[m + 1 + j];

    //merge temp arrays back into pairs
    
    //initial indexes: i: L[], j: R[], k: merged array
    i = 0; j = 0; k = l;
    
    while (i< nl && j < nr)
    {
        if (preferences[pairs[i].winner][pairs[i].loser] >= preferences[pairs[j].winner][pairs[j].loser])
        {
            pairs[k] = L[i];
            i++;
        }
        else
        {
            pairs[k] = R[j];
            j++;
        }
        k++;
    }
    //copy and remaining elements of L[] or R[] (if any)
    while (i < nl)
    {
        pairs[k] = L[i];
        i++;
        k++;
    }
    while (i < nr)
    {
        pairs[k] = R[j];
        j++;
        k++;
    }
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    // Number of ith candidates locked wins and losses: W[i] , L[i]
    int W[candidate_count]; int L[candidate_count];

    // Clear W[] amd L[] array
    for (int i = 0; i < candidate_count; i++)
    {
        W[i] = 0;
        L[i] = 0;
    }

    //Lock pairs
    for (int i = 0; i < pair_count; i++)
    { 
        locked[pairs[i].winner][pairs[i].loser] = true;
        W[pairs[i].winner]++;
        L[pairs[i].loser]++;
    
        //if last lock creates cycle
        if (i == (pair_count - 1) && cycle_check(W, L))
            locked[pairs[i].winner][pairs[i].loser] = false;
    }
    return;
   
}

//checks if lock array creates cycle
bool cycle_check(int W[], int L[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!(W[i] == 1) || !(L[i] == 1))
            return false;
    }
    return true;
}


// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {   
        int nlosses = 0; //number of locked losses of candidate i

        //count candidate i locked losses
        for ( int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
                nlosses++;
        }
        // Condorcet winner (i.e root of graph) only candidate with no loss
        if (nlosses == 0)
        {   
            printf("%s\n", candidates[i]);
            return;
        }
    }
}   

