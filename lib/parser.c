#include "../include/parser.h"
#include <sys/time.h>

/**
 * Creates a new iterator over the batch operations file given by parameter.
 * Returns a pointer to the new iterator or NULL in case of error
 */
iterator * new_iterator(char * file_name){
	iterator * iter = (iterator*) malloc (sizeof(iterator));
	iter->file = fopen(file_name, "r");
	if(iter->file == NULL){
		printf("ERROR: opening batch operations file. \n");
		return NULL;
	}
	return iter;
}

/**
 * Destroys a previously created iterator (pointer to the iterator given
 * by parameter).
 */
void destroy_iterator(iterator * iter){
	fclose(iter->file);
	free(iter);
}

/**
 * This function parses the next operation included in a batch operations
 * file. Receives by parameter pointers to the variables that should be filled
 * (id, type, num_shares, and price).
 * Returns a negative value in case of error and positive values in other case.
 */
int next_operation(iterator * iter, char * id, int * type, int * num_shares, int * price){
	// Initialize char buffers
	memset(id,11,0);

	int res = fscanf(iter->file, "%s %d %d %d\n",id,type,num_shares,price);
	//printf("id: %s, type: %d, num_shares: %d, price: %lf\n",id,*type,*num_shares,*price);
	
	return res;
}

/**
 * Prints a trace with proper format
 */
void print_trace(char * msg){
	
	char file_name[32];
	char buffer[2048];
	file_name[0] = '\0';
	
	sprintf(file_name, "%ld",pthread_self());
	strcat(file_name, ".trace");
	
	umask(0000);
	int fd = open(file_name, O_CREAT | O_APPEND | O_WRONLY, 0666);
	if(fd < 0){
		// Fatal error
		exit(-1);
	}
	
	struct timeval t1;
	gettimeofday(&t1, NULL);
	
	sprintf(buffer,"%ld\t%ld\t%s",t1.tv_sec,t1.tv_usec,msg);
	write(fd, buffer, strlen(buffer));
	
	close(fd);
	printf("%s",buffer);
}

/* 
 * EXAMPLE OF USE
	char id[11];
	int type;
	int num_shares;
	double price;
	iterator *iter;
	iter = new_iterator("file");
	
	while(next_operation(iter,id,&type,&num_shares,&price) >= 0){
		printf("id: %s, type: %d, num_shares: %d, price: %lf\n",id,type,num_shares,price);
	}

	destroy_iterator(iter);
*/
