#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../include/concurrency_layer.h"
	
pthread_mutex_t mutex;
pthread_mutex_t mutex2;

pthread_cond_t esperaBroker;   
pthread_cond_t esperaOperation;

int n_elementos=0;
int n_lectores = 0;


void init_concurrency_mechanisms(){
	
    	pthread_mutex_init(&mutex, NULL);
    	pthread_mutex_init(&mutex2, NULL);

    	pthread_cond_init(&esperaBroker, NULL);
    	pthread_cond_init(&esperaOperation, NULL);
 
}


void destroy_concurrency_mechanisms(){
    
    	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);

    	pthread_cond_destroy(&esperaBroker);
   	pthread_cond_destroy(&esperaOperation);   

}

   
void* broker(void *args){
    	  
	
	broker_info *info_b1 = (broker_info *) args;
	
	char id[11];
	int type;
	int num_shares;
	int price;
	iterator *iter;
	iter = new_iterator(info_b1->batch_file);
	

	//Mientras haya operaciones pendientes sigue
	
	while(next_operation(iter,id,&type,&num_shares,&price) >= 0){
		//printf("id: %s, type: %d, num_shares: %d, price: %lf\n",id,type,num_shares,price);
		
		//Nueva operacion
		operation operacion;
		new_operation(&operacion, id, type, num_shares,price);
		
		pthread_mutex_lock(&mutex);
		while (n_elementos > 9){
           		 pthread_cond_wait(&esperaBroker, &mutex);
		}

		//Proceso de encolar
		enqueue_operation(info_b1->market->stock_operations, &operacion);

		n_elementos++;
		pthread_cond_signal(&esperaOperation);
		pthread_mutex_unlock(&mutex);
		
	}

	
	destroy_iterator(iter);
	
   	pthread_exit(NULL);//Fin del thread
    	return NULL;
    
}

void* operation_executer(void *args){
	
	exec_info *info_ex1 = (exec_info *) args; 
	operation operacion;
	
	pthread_mutex_lock(info_ex1->exit_mutex);
	int exit = *info_ex1->exit;
	pthread_mutex_unlock(info_ex1->exit_mutex);	


	while(exit == 0 || operations_queue_empty(info_ex1->market->stock_operations) == 0){ //Mientras exit este a 0 o la cola no este vacia
		
		while(operations_queue_empty(info_ex1->market->stock_operations) == 0){
			
			pthread_mutex_lock(&mutex);
			while (n_elementos == 0){
           		 	pthread_cond_wait(&esperaBroker, &mutex);
			}

			//Proceso de desencolar
			dequeue_operation(info_ex1->market->stock_operations, &operacion);
			process_operation(info_ex1->market, &operacion);

			n_elementos--;
			pthread_cond_signal(&esperaBroker);
			pthread_mutex_unlock(&mutex);

		}
		
		pthread_mutex_lock(info_ex1->exit_mutex);
		exit = *info_ex1->exit;
		pthread_mutex_unlock(info_ex1->exit_mutex);
		
		
	}
		
    	pthread_exit(NULL);//Fin del thread
    	return NULL;
    
}

void* stats_reader(void *args){

	
	reader_info *info_re1 = (reader_info *) args; 
	
	pthread_mutex_lock(info_re1->exit_mutex);
	int exit = *info_re1->exit;
	pthread_mutex_unlock(info_re1->exit_mutex);	
	
	while(exit == 0){ //Mientras exit este a 0

		pthread_mutex_lock(&mutex2);
 		n_lectores++;
 		if (n_lectores == 1){
 			pthread_mutex_lock(&mutex);
		}
 		pthread_mutex_unlock(&mutex2);

		print_market_status(info_re1->market);
		

 		pthread_mutex_lock(&mutex2);
 		n_lectores--;
 		if (n_lectores == 0) {
			pthread_mutex_unlock(&mutex);
		}
 		pthread_mutex_unlock(&mutex2); 
	
		usleep(info_re1->frequency);

		pthread_mutex_lock(info_re1->exit_mutex);
		exit = *info_re1->exit;
		pthread_mutex_unlock(info_re1->exit_mutex);


	}
	
   	pthread_exit(NULL);//Fin del thread
    	return NULL;
    
}

