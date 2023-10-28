#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

struct score
{
    int result;
    int diagonal;
    int upper;
    int side;
};

// Function to calculate maximum value between three integers
int maximum(int diagonal, int upper, int side)
{
    if (diagonal > upper)
    {
        if (diagonal > side)
        {
            return diagonal;
        }
        else
        {
            return side;
        }
    }
    else
    {
        if (upper > side)
        {
            return upper;
        }
        else
        {
            return side;
        }
    }
}

int allowed_maximum(int diagonal_cost, int diagonal, int upper_cost, int upper, int side_cost, int side)
{
    if (diagonal == 0)
    {
        diagonal_cost = 0;
    }
    if (upper == 0)
    {
        upper_cost = 0;
    }
    if (side == 0)
    {
        side_cost = 0;
    }

    if (diagonal_cost > upper_cost)
    {
        if (diagonal_cost > side_cost)
        {
            return diagonal_cost;
        }
        else
        {
            return side_cost;
        }
    }
    else
    {
        if (upper_cost > side_cost)
        {
            return upper_cost;
        }
        else
        {
            return side_cost;
        }
    }
}

int allowed_minimum(int diagonal_cost, int diagonal, int upper_cost, int upper, int side_cost, int side)
{
    if ((diagonal == 0) || (diagonal_cost < 0))
    {
        diagonal_cost = INT_MAX;
    }
    if ((upper == 0) || (upper_cost < 0))
    {
        upper_cost = INT_MAX;
    }
    if ((side == 0) || (side_cost < 0))
    {
        side_cost = INT_MAX;
    }
    if (diagonal_cost <= upper_cost)
    {
        if (diagonal_cost <= side_cost)
        {
            return diagonal_cost;
        }
        else
        {
            return side_cost;
        }
    }
    else
    {
        if (upper_cost <= side_cost)
        {
            return upper_cost;
        }
        else
        {
            return side_cost;
        }
    }
}

// Choose random char from A, C, G, T
char choose_random(char exclude)
{
    char characters[] = {'A', 'C', 'G', 'T'};

    if (exclude != 'A' && exclude != 'C' && exclude != 'G' && exclude != 'T')
    {
        int random_index = rand() % 4;
        return characters[random_index];
    }
    else
    {
        char choices[3];
        int index = 0;
        for (int i = 0; i < 4; i++)
        {
            if (characters[i] != exclude)
            {
                choices[index] = characters[i];
                index++;
            }
        }

        int random_index = rand() % 3;
        return choices[random_index];
    }
}

// Function to generate maximum cost strings
int generate_maximum_cost_strings(char *X, int m, char *Y, int n)
{
    // Cost matrix
    struct score cost_matrix[m + 1][n + 1];
    int gap_cost;
    int mismatch_cost;
    printf("Insert the gap cost: ");
    scanf("%d", &gap_cost);
    printf("Insert the mismatch cost: ");
    scanf("%d", &mismatch_cost);

    // Initialize Cost matrix
    for (int i = 0; i < m + 1; i++)
    {
        for (int j = 0; j < n + 1; j++)
        {
            if (i == 0)
            {
                cost_matrix[i][j].result = j * gap_cost;
                cost_matrix[i][j].diagonal = 0;
                cost_matrix[i][j].upper = 0;
                cost_matrix[i][j].side = 1;
            }
            else if (j == 0)
            {
                cost_matrix[i][j].result = i * gap_cost;
                cost_matrix[i][j].diagonal = 0;
                cost_matrix[i][j].upper = 1;
                cost_matrix[i][j].side = 0;
            }
            else
            {
                cost_matrix[i][j].result = 0;
                cost_matrix[i][j].diagonal = 0;
                cost_matrix[i][j].upper = 0;
                cost_matrix[i][j].side = 0;
            }
        }
    }

    // Calculate Maximum Cost matrix
    int diagonal_cost = 0;
    int upper_cost = 0;
    int side_cost = 0;
    int is_maximum = 0;
    for (int i = 1; i < m + 1; i++)
    {
        for (int j = 1; j < n + 1; j++)
        {
            diagonal_cost = cost_matrix[i - 1][j - 1].result + (mismatch_cost * 2);
            upper_cost = cost_matrix[i - 1][j].result + gap_cost;
            side_cost = cost_matrix[i][j - 1].result + gap_cost;

            is_maximum = maximum(diagonal_cost, upper_cost, side_cost);
            cost_matrix[i][j].result = is_maximum;
            if (is_maximum == diagonal_cost)
            {
                cost_matrix[i][j].diagonal = 1;
            }
            if (is_maximum == upper_cost)
            {
                cost_matrix[i][j].upper = 1;
            }
            if (is_maximum == side_cost)
            {
                cost_matrix[i][j].side = 1;
            }
        }
    }

    // Print Maximum Cost matrix
    for (int i = 0; i < m + 1; i++)
    {
        for (int j = 0; j < n + 1; j++)
        {
            printf("%d\t", cost_matrix[i][j].result);
            if (j == n)
            {
                printf("\n");
            }
        }
    }

    // Calculate maximum cost
    int maximum_cost = 0;
    int x_position = m;
    int y_position = n;
    int partial_cost = cost_matrix[m][n].result;
    int move_cost = 0;
    int diagonal = 0;
    int upper = 0;
    int side = 0;
    char ACGT1;
    char ACGT2;
    char reversed_X[100] = {'\0'};
    char reversed_Y[100] = {'\0'};
    int position = 0;

    while (partial_cost != 0)
    {
        diagonal_cost = cost_matrix[x_position - 1][y_position - 1].result;
        upper_cost = cost_matrix[x_position - 1][y_position].result;
        side_cost = cost_matrix[x_position][y_position - 1].result;
        diagonal = cost_matrix[x_position][y_position].diagonal;
        upper = cost_matrix[x_position][y_position].upper;
        side = cost_matrix[x_position][y_position].side;
        move_cost = allowed_minimum(diagonal_cost, diagonal, upper_cost, upper, side_cost, side);
        if ((move_cost == diagonal_cost) && (diagonal == 1))
        {
            x_position--;
            y_position--;
            if ((partial_cost - move_cost) == (mismatch_cost * 2))
            {
                // Mismatch
                ACGT1 = choose_random('0');
                ACGT2 = choose_random(ACGT1);
                reversed_X[position] = ACGT1;
                reversed_Y[position] = ACGT2;
            }
            else
            {
                // Match
                ACGT1 = choose_random('0');
                reversed_X[position] = ACGT1;
                reversed_Y[position] = ACGT1;
            }
        }
        else if ((move_cost == upper_cost) && (upper == 1))
        {
            // Gap in Y
            x_position--;
            reversed_X[position] = choose_random('0');
            reversed_Y[position] = '_';
        }
        else
        {
            // Gap in X
            y_position--;
            reversed_X[position] = '_';
            reversed_Y[position] = choose_random('0');
        }
        maximum_cost += (partial_cost - move_cost);
        partial_cost = move_cost;
        position++;
    }

    // Reverse aligned strings
    int len_X = strlen(reversed_X);
    int len_Y = strlen(reversed_Y);
    for (int i = 0; i < len_X / 2; i++)
    {
        char temp = reversed_X[i];
        reversed_X[i] = reversed_X[len_X - 1 - i];
        reversed_X[len_X - 1 - i] = temp;
    }
    for (int i = 0; i < len_Y / 2; i++)
    {
        char temp = reversed_Y[i];
        reversed_Y[i] = reversed_Y[len_Y - 1 - i];
        reversed_Y[len_Y - 1 - i] = temp;
    }

    printf("The maximum cost strings are:\nX -->\t%s\nY -->\t%s\n", reversed_X, reversed_Y);

    return maximum_cost;
}

int main()
{
    int m = 10;
    int n = 10;
    char X[m];
    char Y[n];
    int result;
    printf("Assignment 1 - Bonus\nThe predefined values are:\nm -->\t%d\nn -->\t%d\nTo change them, modify the code.\n", m, n);
    if (m <= n)
    {
        result = generate_maximum_cost_strings(X, m, Y, n);
    }
    else
    {
        result = generate_maximum_cost_strings(Y, n, X, m);
    }
}