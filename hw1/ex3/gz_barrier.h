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
	volatile int     n;
	volatile int     nthr;
	volatile int     spin;
	pthread_cond_t  *release_threads;
	pthread_mutex_t *mutex;
	pthread_cond_t  *next_wait;
} barrier_t;

int barrier_init(barrier_t *b, int nthr);
int barrier_wait(barrier_t *b);
int barrier_destroy(barrier_t *b);

#endif

