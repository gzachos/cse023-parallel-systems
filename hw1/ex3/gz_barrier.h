/* gz_barrier.h
 * ------------
 * Custom barrier implementation
 * 
 * Developed by George Z. Zachos
 */

#ifndef __GZ_BARRIER
#define __GZ_BARRIER	1

#include <pthread.h>

typedef struct barrier_s {
	unsigned int     init_count;
	unsigned int     arrived;
	unsigned int     left;
	pthread_cond_t  *release_threads;
	pthread_cond_t  *next_bar;
	pthread_mutex_t *mutex;
} barrier_t;

int barrier_init(barrier_t *barrier, unsigned int nthr);
int barrier_wait(barrier_t *barrier);
int barrier_destroy(barrier_t *barrier);

#endif

