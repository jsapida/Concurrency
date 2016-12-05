#ifndef PMMS_H
#define PMMS_H

/* Function createMThreads
*		@param int m 
*			- specifies the number of threads to be created
*		@param pthread_t *threads 
*			- represents the threads to be created 
*/
void createMThreads(int m, pthread_t *threads);


/* Function consumerThread
*		@param int *total 
*			- stores the total value calculated from all subtotals
*/
void consumerThread(int *total);

/* Function producerThread 
*		- The function that the thread calls upon creation
*/
void *producerThread(void *param);
#endif
