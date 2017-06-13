#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct iterator{
	FILE * file;
}iterator;

/**
 * Creates a new iterator over the batch operations file given by parameter.
 * Returns a pointer to the new iterator or NULL in case of error
 */
iterator * new_iterator(char * file_name);

/**
 * Destroys a previously created iterator (pointer to the iterator given
 * by parameter).
 */
void destroy_iterator(iterator * iter);

/**
 * This function parses the next operation included in a batch operations
 * file. Receives by parameter pointers to the variables that should be filled
 * (id, type, num_shares, and price).
 * Returns a negative value in case of error and positive values in other case.
 */
int next_operation(iterator * iter, char * id, int * type, int * num_shares, int * price);

/**
 * Prints a trace with proper format
 */
void print_trace(char * msg);