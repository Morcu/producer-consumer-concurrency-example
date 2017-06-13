#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define ID_LENGTH 11
#define ENQUEUE_SIMULATION 1000
#define DEQUEUE_SIMULATION 1000

typedef struct operation{
	char id[ID_LENGTH];		// Stock ID
	int type; 			// BUY or SELL
	int num_shares;			// no. shares to operate with (buy or sell)
	int share_price;		// Price to pay per share in the operation
}operation;
/* 
* Queue has five properties. 
* capacity maximum number of elements 
* Size current 
* items items 
* front index of first element 
* rear is the index of last element
*/
typedef struct operations_queue
{
	int capacity;
	int size;
	int front;
	int rear;
	unsigned long operations_enqueued;
	unsigned long operations_dequeued;
	operation * operations;
}operations_queue;

/* new_queue takes argument the maximum number of elements, creates
  a new queue according to it and returns a pointer to the queue. */
operations_queue * new_operations_queue(int max_items);

/**
 * Frees queue memory resources
 */
void delete_operations_queue(operations_queue *q);

/**
 * Checks if the queue is empty
 * Returns 1 if empty and 0 if not
 */
int operations_queue_empty(operations_queue *q);

/**
 * Checks if the queue is full
 * Returns 1 if full and 0 if not
 */
int operations_queue_full(operations_queue *q);

/* 
 * Returns the operation in front of the queue 
 * (copying the operation into the pointer given by parameter)
 * and removes it from the queue 
 * Returns 0 if the dequeue was correct
 * -1 in case of error (e.g. queue empty)
 */
int dequeue_operation(operations_queue *q, operation * op);

/* 
 * Puts a new operation in the back of the queue
 * (copying the operation given by parameter into the queue)
 * Returns 0 if the enqueue was correct and
 * -1 in case of error (e.g. queue is full).
 * The operation should be malloc'd by the caller
 */
int enqueue_operation(operations_queue *q, operation * op);

/*
 * Creates a new operation based on the information given by
 * parameter. This function DOES NOT allocate memory for the new operation
 */
void new_operation(operation *op, char id[ID_LENGTH], int type, int num_shares, int share_price);