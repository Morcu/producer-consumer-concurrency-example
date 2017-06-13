#include "stock_market_lib.h"
#include <pthread.h>

typedef struct broker_info{
	char batch_file[256];
	stock_market * market;
}broker_info;

typedef struct exec_info{
	int *exit;
	stock_market * market;
	pthread_mutex_t *exit_mutex;
}exec_info;

typedef struct reader_info{
	int *exit;
	stock_market * market;
	pthread_mutex_t *exit_mutex;
	unsigned int frequency;
}reader_info;

void init_concurrency_mechanisms();

void destroy_concurrency_mechanisms();

void* broker(void * args);

void* operation_executer(void * args);

void* stats_reader(void * args);

