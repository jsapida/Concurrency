#ifndef PMMS_H
#define PMMS_H

/* Function createMProcesses
*		@param int m 
*			- specifies the number of children to be forked
*		@param pid_t *pid 
*			- stores the pid for all the processes 
*/
void createMProcesses(int m, pid_t *pid);

/* Function consumerProcess
*		@param sem_t *full, *empty, *mutex
*			- pertains to the semaphores used for the algorithm
*		@param int *subtotal
*			- pointer to the shared memory region containing the 
*			  data structure for the subtotal 
*			- the first element contains the value calculated and
*			  the second element contains the producer pid
*		@param int *total 
*			- stores the total value calculated from all subtotals
*/
void consumerProcess(sem_t *full, sem_t *mutex, sem_t *empty, int *subtotal, int *total);
#endif
