/* gz_barrier.c
 * ------------
 * Custom barrier implementation
 * 
 * Developed by George Z. Zachos
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "gz_barrier.h"


int barrier_init(barrier_t *b, int nthr)
{
	if (nthr < 0)
		return EXIT_FAILURE;
	b->nthr = b->n = b->spin = nthr;

	b->release_threads = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
	if (!b->release_threads)
	{
		perror("malloc");
		return errno;
	}

	b->next_wait = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
	if (!b->next_wait)
	{
		free(b->release_threads);
		perror("malloc");
		return errno;
	}

	b->mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	if (!b->mutex)
	{
		free(b->release_threads);
		free(b->next_wait);
		perror("malloc");
		return errno;
	}

	pthread_cond_init(b->release_threads, NULL);
	pthread_cond_init(b->next_wait, NULL);
	pthread_mutex_init(b->mutex, NULL);

	return EXIT_SUCCESS;
}


int barrier_wait(barrier_t *b)
{
	pthread_mutex_lock(b->mutex);
	while (b->spin != b->nthr)
		pthread_cond_wait(b->next_wait, b->mutex);
	(b->n)--;
	while (b->n != 0)
		pthread_cond_wait(b->release_threads, b->mutex);
	(b->spin)--;
	if (b->spin == b->nthr-1)
		pthread_cond_broadcast(b->release_threads);
	if (b->spin == 0)
	{
		b->n = b->spin = b->nthr;
		pthread_cond_broadcast(b->next_wait);
	}
	pthread_mutex_unlock(b->mutex);

	return EXIT_SUCCESS;
}


int barrier_destroy(barrier_t *b)
{
	free(b->release_threads);
	free(b->mutex);
	free(b->next_wait);

	b = NULL;

	return EXIT_SUCCESS;
}

