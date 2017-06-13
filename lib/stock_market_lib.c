#include "../include/stock_market_lib.h"

/**
 * This function initializes the stock market and all the
 * stocks to default values ("\0" for strings and 0 for values)
 */
int init_market(stock_market * market, char * file_name){
	int i;
	
	// Initialize each stock of the stock market
	for(i = 0; i < NUM_STOCKS; i++){
		memset(market->stocks[i].id, 0, ID_LENGTH);
		memset(market->stocks[i].name, 0, STOCK_NAME_LENGTH);
		market->stocks[i].total_value = 0;
		market->stocks[i].current_share_value = 0;
		market->stocks[i].total_shares = 0;
	}
	
	// Initialize stock market characteristics
	market->total_value = 0.0;
	market->avg_value = 0.0;
	market->num_active_stocks = 0;
	
	// Initialize stock_operations queue
	market->stock_operations = new_operations_queue(MAX_OPERATIONS);
	
	// Read file with stocks and create new stocks
	FILE * file;
	char id[ID_LENGTH];
	char name[STOCK_NAME_LENGTH];
	int current_share_value;
	int total_shares;
	
	
	file = fopen(file_name, "r");
	if(file == NULL){
		return -1;
	}
	
	// Read file line by line and create stock
	while(fscanf(file, "%s %s %d %d\n",id,name,&total_shares,&current_share_value) >= 0){
		new_stock(market, id, name, current_share_value, total_shares);
	}
	
	return 0;
}

/**
 * Frees market resources
 */
void delete_market(stock_market * market){
	delete_operations_queue(market->stock_operations);
}

/**
 * This function initializes the stock market and all the
 * stocks to default values ("\0" for strings and 0.0 for values)
 * Following, it uses the information in the file given by parameter
 * to create new stocks
 * Returns 0 with correct functionality or -1 in case of error.
 */
int new_stock(stock_market * market, char id[ID_LENGTH], char name[STOCK_NAME_LENGTH], int current_share_value, int total_shares){
	int i;
	// Look up the first stock space available
	for(i = 0; i < NUM_STOCKS; i++){
		if(market->stocks[i].id[0] == '\0'){
			break;
		}
	}
	if(i == NUM_STOCKS){
		return -1; // No stock space availble
	}
	
	memcpy(market->stocks[i].id, id, ID_LENGTH);
	memcpy(market->stocks[i].name, name, STOCK_NAME_LENGTH);
	market->stocks[i].current_share_value = current_share_value;
	market->stocks[i].total_shares = total_shares;
	market->stocks[i].total_value = current_share_value * total_shares;
	market->num_active_stocks++;
	update_market_statistics(market);
	
	return 0;
}

/**
 * Search the stock corresponding with the ID given by parameter
 * Returns a pointer to the stock if iss found or NULL in any other case
 */
stock * lookup_stock(stock_market * market, char id[ID_LENGTH]){
	int i = 0;
	
	if(id[0] == '\0'){
		return NULL; // Empty lookup
	}
	
	while(i < NUM_STOCKS && strncmp(id, market->stocks[i].id, ID_LENGTH)){
		i++;
	}
	
	if(i == NUM_STOCKS){
		return NULL;
	}else{
		return &(market->stocks[i]);
	}
}

/*
 * Updates the statistics (total_value and avg_value) of a given stock market
 */
void update_market_statistics(stock_market * market){
	int total = 0;
	int i;
	for(i = 0; i < NUM_STOCKS; i++){
		total = total + market->stocks[i].total_value;
	}
	market->total_value = total;
	market->avg_value = total / market->num_active_stocks;
	printf("STATS market: total_value: %d, avg_value: %f\n",market->total_value, market->avg_value);
}

/**
 * Dequeues an operation and processes it.
 * Returns 0 if the operation is queued sucessfully and -1 in case of error.
 * MUST FREE THE OPERATION PREVIOUSLY MALLOC'D
 */
int process_operation(stock_market * market, operation * op){

	stock * st = lookup_stock(market, op->id);
	
	if(st == NULL){
		printf("ERROR: stock with ID %s not found \n",op->id);
		return -1; // Stock not found with this ID
	}
	
	if(op->type == SELL){
		if(op->num_shares >= st->total_shares || op->num_shares == 0){
			printf("ERROR: SELL operation over an invalid number of shares (%d >= %d) \n",
				op->num_shares, st->total_shares);
			return -1;
		}else{
			// When selling shares, we substract the value of the sold shares
			// And recalculate the new value of the company and of each remaining shares
			// 1.- Calculate operation value (num_shares sold * share price paid)
			double operation_value = op->num_shares * op->share_price;
			// 2.- Calculate new company value
			st->total_value = st->total_value - operation_value;
			// 3.- Calculate the new number of shares
			st->total_shares = st->total_shares - op->num_shares;
			// 4.- Calculate new share value
			st->current_share_value = st->total_value / st->total_shares;
			printf("OPERATION: SELL STATS stock: id: %s, value: %d, num_shares: %d, share_value: %d\n",
				st->id,st->total_value,st->total_shares,st->current_share_value);
			
			// Last step: update market statistics
			update_market_statistics(market);
		}
	}else if(op->type == BUY){
		if(op->num_shares == 0){
			printf("ERROR: BUY operation over an invalid number of shares (0 shares) \n");
			return -1;
		}
		// When buying shares, we add the value of the bought shares
		// And recalculate the new value of the company and of each new number of shares
		// 1.- Calculate operation value (num_shares sold * share price paid)
		double operation_value = op->num_shares * op->share_price;
		// 2.- Calculate new company value
		st->total_value = st->total_value + operation_value;
		// 3.- Calculate the new number of shares
		st->total_shares = st->total_shares + op->num_shares;
		// 4.- Calculate new share value
		st->current_share_value = st->total_value / st->total_shares;
		printf("OPERATION: BUY STATS stock: id: %s, value: %d, num_shares: %d, share_value: %d\n",
			st->id,st->total_value,st->total_shares,st->current_share_value);
		
		// Last step: update market statistics
		update_market_statistics(market);
	}else{
		return -1; // Invalid operation type
	}
	
	return 0;
}

/**
 * Prints the current status of the stock market given by parameter
 */
void print_market_status(stock_market * market){
	int i;
	printf("========== MARKET STATUS REPORT START ==========\n");
	
	update_market_statistics(market);
	printf("QUEUE STATS: enqueued: %ld, dequeued: %ld \n",
			market->stock_operations->operations_enqueued,market->stock_operations->operations_dequeued);
	usleep(READ_SIMULATION);
	printf("ID\tCOMPANY\tSHARES\tSHARE VALUE\tTOTAL VALUE\n");
	for(i = 0; i < NUM_STOCKS; i++){
		if(market->stocks[i].id[0] != '\0'){
			printf("%s\t%s\t%d\t%d\t%d\n",market->stocks[i].id,market->stocks[i].name,
				market->stocks[i].total_shares,market->stocks[i].current_share_value,
				market->stocks[i].total_value);
		}
	}
	usleep(READ_SIMULATION);
	update_market_statistics(market);
	printf("QUEUE STATS: enqueued: %ld, dequeued: %ld \n",
			market->stock_operations->operations_enqueued,market->stock_operations->operations_dequeued);
	printf("========== MARKET STATUS REPORT END ==========\n");
}