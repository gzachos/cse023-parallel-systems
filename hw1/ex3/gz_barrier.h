/* gz_barrier.h
 * ------------
 * Custom barrier implementation
 * 
 * Developed by George Z. Zachos
 */

#ifndef __GZ_BARRIER
#define __GZ_BARRIER

#include <pthread.h>

typedef struct barrier_s {
	unsigned int     left;
	unsigned int     init_count;
	unsigned int     curr_count;
	pthread_cond_t  *release_threads;
	pthread_mutex_t *mutex;
	pthread_cond_t  *next_bar;
} barrier_t;

int barrier_init(barrier_t *barrier, unsigned int nthr);
int barrier_wait(barrier_t *barrier);
int barrier_destroy(barrier_t *barrier);

#endif

