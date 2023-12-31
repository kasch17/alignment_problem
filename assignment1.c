#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// Cost matrix elements
struct score
{
    int result;
    int diagonal;
    int upper;
    int side;
};

// Function to calculate minimum value between three integers
int minimum(int diagonal, int upper, int side)
{
    if (diagonal <= upper)
    {
        if (diagonal <= side)
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
        if (upper <= side)
        {
            return upper;
        }
        else
        {
            return side;
        }
    }
}

// Function to calculate minimum value between three integers, but only if selected as possible moves and non-negative
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

int compute_alignment(char *X, int X_length, char *Y, int Y_length)
{
    // Cost matrix
    struct score cost_matrix[X_length + 1][Y_length + 1];
    int gap_cost;
    int mismatch_cost;
    printf("Insert the gap cost: ");
    scanf("%d", &gap_cost);
    printf("Insert the mismatch cost: ");
    scanf("%d", &mismatch_cost);

    // Initialize Cost matrix
    for (int i = 0; i < X_length + 1; i++)
    {
        for (int j = 0; j < Y_length + 1; j++)
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

    // Calculate Cost matrix
    int diagonal_cost = 0;
    int upper_cost = 0;
    int side_cost = 0;
    int is_minimum = 0;
    for (int i = 1; i < X_length + 1; i++)
    {
        for (int j = 1; j < Y_length + 1; j++)
        {
            if (X[i - 1] != Y[j - 1])
            {
                diagonal_cost = cost_matrix[i - 1][j - 1].result + (mismatch_cost * 2);
            }
            else
            {
                diagonal_cost = cost_matrix[i - 1][j - 1].result;
            }
            upper_cost = cost_matrix[i - 1][j].result + gap_cost;
            side_cost = cost_matrix[i][j - 1].result + gap_cost;

            is_minimum = minimum(diagonal_cost, upper_cost, side_cost);
            cost_matrix[i][j].result = is_minimum;
            if (is_minimum == diagonal_cost)
            {
                cost_matrix[i][j].diagonal = 1;
            }
            if (is_minimum == upper_cost)
            {
                cost_matrix[i][j].upper = 1;
            }
            if (is_minimum == side_cost)
            {
                cost_matrix[i][j].side = 1;
            }
        }
    }

    // Print Cost matrix
    printf("+\t_\t");
    for (int i = 0; i < Y_length; i++)
    {
        printf("%c\t", Y[i]);
        if (i == Y_length - 1)
        {
            printf("\n");
        }
    }
    for (int i = 0; i < X_length + 1; i++)
    {
        if (i == 0)
        {
            printf("_\t");
        }
        else
        {
            printf("%c\t", X[i - 1]);
        }
        for (int j = 0; j < Y_length + 1; j++)
        {
            printf("%d\t", cost_matrix[i][j].result);
            if (j == Y_length)
            {
                printf("\n");
            }
        }
    }

    // Calculate minimum cost
    int minimum_cost = 0;
    int x_position = X_length;
    int y_position = Y_length;
    int partial_cost = cost_matrix[X_length][Y_length].result;
    int move_cost = 0;
    int diagonal = 0;
    int upper = 0;
    int side = 0;
    char aligned_X[100] = {'\0'};
    char aligned_Y[100] = {'\0'};
    int position = 0;

    while ((partial_cost != 0))
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
                aligned_X[position] = '*';
                aligned_Y[position] = '*';
            }
            else
            {
                // Match
                aligned_X[position] = X[x_position];
                aligned_Y[position] = Y[y_position];
            }
        }
        else if ((move_cost == upper_cost) && (upper == 1))
        {
            // Gap in Y
            x_position--;
            aligned_X[position] = X[x_position];
            aligned_Y[position] = '_';
        }
        else
        {
            // Gap in X
            y_position--;
            aligned_X[position] = '_';
            aligned_Y[position] = Y[y_position];
        }
        minimum_cost += (partial_cost - move_cost);
        partial_cost = move_cost;
        position++;
    }
    while ((x_position) && (y_position) && (partial_cost == 0))
    {
        x_position--;
        y_position--;
        aligned_X[position] = X[x_position];
        aligned_Y[position] = Y[y_position];
        position++;
    }

    // Reverse aligned strings
    int len_X = strlen(aligned_X);
    int len_Y = strlen(aligned_Y);
    for (int i = 0; i < len_X / 2; i++)
    {
        char temp = aligned_X[i];
        aligned_X[i] = aligned_X[len_X - 1 - i];
        aligned_X[len_X - 1 - i] = temp;
    }
    for (int i = 0; i < len_Y / 2; i++)
    {
        char temp = aligned_Y[i];
        aligned_Y[i] = aligned_Y[len_Y - 1 - i];
        aligned_Y[len_Y - 1 - i] = temp;
    }

    printf("The aligned strings are:\nX -->\t%s\nY -->\t%s\n", aligned_X, aligned_Y);

    return minimum_cost;
}

int main()
{
    char X[] = "GATTACA";
    char Y[] = "GTCGACGCA";
    printf("Assignment 1\nThe predefined strings are:\nX -->\t%s\nY -->\t%s\nTo change them, modify the code.\n", X, Y);
    int minimum_cost = 0;
    if (strlen(X) < strlen(Y))
    {
        minimum_cost = compute_alignment(X, strlen(X), Y, strlen(Y));
    }
    else
    {
        minimum_cost = compute_alignment(Y, strlen(Y), X, strlen(X));
    }
    printf("Cost: %d\n", minimum_cost);
}