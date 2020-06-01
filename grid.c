// Ivan
// Program calculate all correct solutions to Matt Parkers Math Puzzle 7
// Works on the input integer value n, (n = 6 for submitable part of MPMP7)


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define FIRST 'A'
#define EMPTY '0'


// Get the input from the user for num tokens (equal to grid length) and num threads
void get_input(int argc, char **argv, long int *n) {
	if (argc != 2) {
		fprintf(stderr, "Usage: (pos int) num_tokens\n");
		exit(1);
	}
	
	if ((*n = strtol(argv[1], NULL, 10)) <= 0) {
		fprintf(stderr, "Usage: (pos int) num_tokens\n");
		exit(1);
	}
}


// Displays the current grid 
void display_grid(char *grid, long int n) {
	// Loop over the grid and display each character
	for (int i = 0; i < n * n; i++) {
		fprintf(stderr, "%c ", grid[i]);

		if ((i + 1) % n == 0) {
			fprintf(stderr, "\n");
		}
	}
	
	fprintf(stderr, "=======================\n\n");
}


// Sets the rest of the tokens recursively
// grid is the array representation of the grid
// distances is the array of arrays of distances
// token val is which token is being set, also this token's index into distances
// prev_token_index is the index where the previous token is 
// this is useful because there is no need to try to put a token at grid spots before that
// index because those spots have been checked already
// n is n
void recursive(char *grid, int **distances, int token_val, long int n, int prev_token_index) {
	int index;
	// Set this token by looping over the grid
	for (index = prev_token_index; index < n * n; index++) {
		// This is a free grid spot
		if (grid[index] == EMPTY) {
			grid[index] = FIRST + 1 + token_val; // B token has val 0

			// Counter for the index into this token's distance array
			int dist_array_index = 0;
			
			// Loop over the grid to calculate all the distances
			for (int other_token = 0; other_token < n * n; other_token++) {
				// Check if there is token at this spot
				if (grid[other_token] != EMPTY) {
					// Calculate distance from current token to this token
					int index_row = ((index + 1) + n - 1) / n;
					int other_row = ((other_token + 1) + n - 1) / n;

					int index_col = (index % n) + 1;
					int other_col = (other_token % n) + 1;

					int row_dist = index_row - other_row;
					int col_dist = index_col - other_col;
					
					// This should be square rooted to get the actual correct distance
					// but since we are only comparing distances it doesn't matter
					int total_dist = (row_dist * row_dist) + (col_dist * col_dist); 

					// Store this distance at the correct index of the array
					distances[token_val][dist_array_index] = total_dist;
					dist_array_index ++;
					
					// We have checked every other token so we can break early
					if (dist_array_index == token_val + 1) {
						break;
					}
				}
			}

			// Loop over this token's distance array
			for (int i = 0; i < token_val + 1; i++) {
				// Loop over the rest of the distance arrays
				for (int array = 0; array < token_val + 1; array++) {
					// Loop over each distance in each distance array
					for (int dist = 0; dist < array + 1; dist++) {
						// Distance should never be 0
						if (distances[token_val][i] == 0 || distances[array][dist] == 0) {
							fprintf(stderr, "Error: Distances 0\n");
							exit(1);
						}

						// Check if we are comparing the same grid spot
						if (!(token_val == array && i == dist)) {
							// Check if the distances are the same
							if (distances[token_val][i] == distances[array][dist]) {
								// Token can't go here so we check the next place it can go
								goto next_token;
							}
						}
					}
				}
			}

			// Check if this is the last token to place everything works so we display the grid
			// Then as above we clear this token and return
			// This is technically the base case lol
			if (token_val == n-2) {
				display_grid(grid, n);

				for (int i = 0; i < token_val + 1; i++) {
					distances[token_val][i] = 0;
				}
				grid[index] = EMPTY;
				return;
			}

			// If this isn't the last token we recurse onto the next token
			recursive(grid, distances, token_val + 1, n, index);

			// Clear this token from the grid so that we can increment this token
			next_token: grid[index] = EMPTY;
		}
	}

	// We have checked all possible places for this token so we clear this token and return from it
	for (int i = 0; i < token_val + 1; i++) {
		distances[token_val][i] = 0;
	}

	grid[index] = EMPTY;
	return;
}


// Does the main calculation
// n = number of tokens and length of grid
void calculate(long int n) {
	// int num_correct = 0; // Keep track of number of correct solutions found

	// Create array representation of the grid
	char *grid = malloc(sizeof(char) * n * n);
	memset(grid, EMPTY, sizeof(char) * n * n);

	// Create array of pointers to each of the distance arrays
	// distances[0] represents distance array of B
	int **distances = malloc(sizeof(int *) * (n - 1));

	// Create distance arrays of the correct size for each token
	for (int i = 1; i < n; i++) {
		int *array = malloc(sizeof(int) * i);
		
		// Zero out each distance array
		for (int j = 0; j < i; j++) {
			array[j] = 0;
		}

		distances[i - 1] = array;

	}

	// Loop over the grid with the first token
	for (int index = 0; index < n * n; index++) {
		// Set this token
		grid[index] = FIRST;

		// Set the rest of the tokens
		recursive(grid, distances, 0, n, index);

		// Remove this token
		grid[index] = EMPTY;
	}

	return;
}


int main(int argc, char **argv) {
	// Get the input for the number size of the grid, and number of threads
	long int n;
	get_input(argc, argv, &n);
	// printf("n: %ld, num_threads: %ld\n", n, num_threads);
	
	// Calculating the grids
	calculate(n);

	return 0;	
}
