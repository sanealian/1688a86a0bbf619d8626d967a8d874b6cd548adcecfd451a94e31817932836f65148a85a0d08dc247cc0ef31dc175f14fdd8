#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#define Y_SIZE 55
#define X_SIZE 100
#define LATENCY ((useconds_t) 1000*150)
#define LETTERS_COUNT 18
const char LETTER_LIST[LETTERS_COUNT] = { 'A', 'B', 'C', 'D', 'E', 'F',
                                      '0', '1', '2', '3', '4', '5',
                                      '6', '7', '8', '9','*','&' };


char get_random_letter ();
void matrix_rand_letters_fill (void * p_matrix, int rows, int cols);
void flush_matrix_toscreen (void * p_matrix, int rows, int cols);
void flush_row_to_screen (int cols, char row[]);
void row_rand_letters_fill (int cols, char row[]);
void start_revolution(void * p_matrix, int rows, int cols);


int main(void)
{

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    printf("%d", w.ws_col);
    char array[Y_SIZE][X_SIZE] = {{0}};

    matrix_rand_letters_fill(&array,Y_SIZE, X_SIZE);
    start_revolution(&array, Y_SIZE, X_SIZE);
    return 0;
}



// ---------------------------------------------------------------------------

void
matrix_rand_letters_fill (void * p_matrix, int rows, int cols)
{
    char * matrix = (char*) p_matrix;
	for (int i=0, j=0; i < rows; ++j)
	{
		matrix[i * cols + j] = LETTER_LIST[ rand() % LETTERS_COUNT ];;
		(j >= cols-1) && ++i && (j = -1);
	}
}

// ---------------------------------------------------------------------------

void
row_rand_letters_fill (int cols, char row[])
{
    for (int j=0; j < cols; ++j)
	{
		row[j] = LETTER_LIST[ rand() % LETTERS_COUNT ];;
	}
}

// ---------------------------------------------------------------------------

void
flush_matrix_toscreen (void * p_matrix, int rows, int cols)
{
    char * matrix = (char*) p_matrix;
	for (int i=0, j=0; i < rows; ++j)
	{
		printf("%c ", matrix[ i * cols + j]);
		(j >= cols-1) && ++i && (j = -1) && printf("\n");
	}
}

// ---------------------------------------------------------------------------

void
flush_row_to_screen (int cols, char row[])
{
    for (int j=0; j < cols; ++j)
        printf("%c ", row[j]);
    printf("\n");
}

// ---------------------------------------------------------------------------

void
start_revolution(void * p_matrix, int rows, int cols)
{
    char * matrix = (char*) p_matrix;
    int times = 0;
    int shift = 0;
    int rows_counter = rows;
    int i_ceil = rows;
    int i = 0, j=0;

    printf("\033[32m");
    while (1)
	{
        // flush_matrix_with_shitf(p_matrix, )
        for (i=rows-shift-1,j=0; rows_counter; ++j)
        {
            printf("%c ", matrix[ i * cols + j]);
			// printf("i: %d | j: %d | shift: %d | i_ceil: %d\n", i, j, shift, i_ceil);
			// Check if current row is printed
			(
				(j >= cols-1)
				// Move to the next row and reset column counter
				&& ++i && (j = -1)
				// Print new line at the end of the current row
				&& printf("\n")
			)
			&&
			(
                ( rows_counter-- ) &&
				(i >= rows) && (i = 0)
			); 
        }
        printf("\033[3J");
        // Reset values
        // --------------------
        rows_counter = rows;
        // Generate new row at the end of the array with current shitf
        i=++shift;
        row_rand_letters_fill (cols, &matrix[rows-shift-1]);
        // Reset shift when it's out the boundary
        (shift >= rows) && (shift = 0);
        ++times;
        usleep(LATENCY);
	}
}

// ---------------------------------------------------------------------------