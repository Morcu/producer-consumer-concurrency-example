#include "../include/operations_queue.h"

/* new_queue takes argument the maximum number of elements, creates
  a new queue according to it and returns a pointer to the queue. */
operations_queue * new_operations_queue(int max_items)
{
	// Create a Queue 
	operations_queue *q;
	q = (operations_queue *)malloc(sizeof(operations_queue));
	// Initialise its properties 
	q->operations = (operation *)malloc(sizeof(operation)*max_items);
	q->size = 0;
	q->capacity = max_items;
	q->front = 0;
	q->rear = -1;
	q->operations_enqueued = 0;
	q->operations_dequeued = 0;
	// Return the pointer 
	return q;
}

/**
 * Frees queue memory resources
 */
void delete_operations_queue(operations_queue *q){
	free(q->operations);
	free(q);
}

/**
 * Checks if the queue is empty
 * Returns 1 if empty and 0 if not
 */
int operations_queue_empty(operations_queue *q){
	if(q->size<=0){
		return 1;
	}else{
		return 0;
	}
}

/**
 * Checks if the queue is full
 * Returns 1 if full and 0 if not
 */
int operations_queue_full(operations_queue *q){
	if(q->size >= q->capacity){
		return 1;
	}else{
		return 0;
	}
}

/* 
 * Returns the element in front of the queue 
 * (copying the element into the pointer given by parameter)
 * and removes it from the queue 
 * Returns 0 if the dequeue was correct
 * -1 in case of error (e.g. queue empty)
 */
int dequeue_operation(operations_queue *q, operation * op)
{
	// If Queue size is zero then it is empty. So we cannot pop 
	if(q->size<=0)
	{
		printf("ERROR: Queue is Empty\n");
		return -1;
	}
	// Removing an element is equivalent to incrementing index of front by one 
	else
	{	
		q->size--;
		memcpy(op, &(q->operations[q->front]), sizeof(operation));
		usleep(DEQUEUE_SIMULATION);
		q->front++;
		/* As we fill elements in circular fashion */
		if(q->front==q->capacity)
		{
			q->front=0;
		}
		usleep(DEQUEUE_SIMULATION);
		q->operations_dequeued++;
		printf("QUEUE: dequeued operation\n");
	}
	return 0;
}

/* 
 * Puts a new element in the back of the queue
 * Returns 0 if the enqueue was correct and
 * -1 in case of error (e.g. queue is full).
 * The element should be malloc'd by the caller
 */
int enqueue_operation(operations_queue *q, operation * op)
{
	/* If the Queue is full, we cannot push an element into it as there is no space for it.*/
	if(q->size == q->capacity)
	{
		printf("ERROR: Queue is Full\n");
		return -1;
	}
	else
	{	
		q->rear = q->rear + 1;
		usleep(ENQUEUE_SIMULATION);
		/* As we fill the queue in circular fashion */
		if(q->rear == q->capacity)
		{
			q->rear = 0;
		}
		usleep(ENQUEUE_SIMULATION);
		q->size++;
		/* Copy the element in its rear side */ 
		memcpy(&(q->operations[q->rear]), op, sizeof(operation));
		q->operations_enqueued++;
		printf("QUEUE: enqueued operation\n");
	}
	return 0;
}

/*
 * Creates a new operation based on the information given by
 * parameter. This function DOES NOT allocate memory for the new operation
 */
void new_operation(operation *op, char id[ID_LENGTH], int type, int num_shares, int share_price){
	memcpy(op->id, id, ID_LENGTH);
	op->type = type;
	op->num_shares = num_shares;
	op->share_price = share_price;
}