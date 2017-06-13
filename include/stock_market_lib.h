#include "parser.h"
#include "operations_queue.h"
#include <stdlib.h>

#define NUM_STOCKS 100
#define MAX_OPERATIONS 10
#define BUY 0
#define SELL 1
#define ID_LENGTH 11
#define STOCK_NAME_LENGTH 256
#define READ_SIMULATION 100000

/**
 * STOCK MARKET DATA STRUCTURES
 */
typedef struct stock{
	 char id[ID_LENGTH]; 		// Stock ID
	 char name[STOCK_NAME_LENGTH]; 	// Company name
	 int total_shares;		// Total number of shares
	 int current_share_value;	// Value of each share of the company (= value / no. shares) 
	 int total_value; 		// Value of the company
}stock;

typedef struct stock_market{
	struct stock stocks[NUM_STOCKS];// Array of stocks in this market
	int total_value;		// Sum of all the stock values
	double avg_value;		// Average value of each stock in this market
	int num_active_stocks;		// Number of active (initialized) stocks
	operations_queue * stock_operations;	// Queue of pending operations
}stock_market;


/**
 * STOCK MARKET FUNCTION HEADERS
 */
/**
 * This function initializes the stock market and all the
 * stocks to default values ("\0" for strings and 0 for values)
 * Following, it uses the information in the file given by parameter
 * to create new stocks
 * Returns 0 with correct functionality or -1 in case of error.
 */
int init_market(stock_market * market, char * file_name);

/**
 * Frees market resources
 */
void delete_market(stock_market * market);

/** 
 * Creates a new stock into the stock market given by parameter.
 * Returns 0 when correct and -1 in case of error (e.g. stock market full)
 */
int new_stock(stock_market * market, char id[ID_LENGTH], char name[STOCK_NAME_LENGTH], int current_share_value, int total_shares);

/**
 * Search the stock corresponding with the ID given by parameter
 * Returns a pointer to the stock if iss found or NULL in any other case
 */
stock * lookup_stock(stock_market * market, char id[ID_LENGTH]);

/*
 * Updates the statistics (total_value and avg_value) of a given stock market
 */
void update_market_statistics(stock_market * market);

/**
 * Dequeues an operation and processes it.
 * Returns 0 if the operation is queued sucessfully and -1 in case of error.
 * MUST FREE THE OPERATION PREVIOUSLY MALLOC'D
 */
int process_operation(stock_market * market, operation * op);

/**
 * Prints the current status of the stock market given by parameter
 */
void print_market_status(stock_market * market);
