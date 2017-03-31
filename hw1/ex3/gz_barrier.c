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


int barrier_init(barrier_t *barrier, unsigned int count)
{
	if (!barrier || count == 0)
		return EINVAL;
	if (barrier->mutex)
		if (pthread_mutex_init(barrier->mutex, NULL) == EBUSY)
			return EBUSY;
	barrier->init_count = barrier->left = barrier->curr_count = count;

	barrier->release_threads = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
	if (!barrier->release_threads)
	{
		perror("malloc");
		return errno; /* =ENOMEM */
	}

	barrier->next_bar = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
	if (!barrier->next_bar)
	{
		free(barrier->release_threads);
		perror("malloc");
		return errno; /* =ENOMEM */
	}

	barrier->mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	if (!barrier->mutex)
	{
		free(barrier->release_threads);
		free(barrier->next_bar);
		perror("malloc");
		return errno; /* =ENOMEM */
	}

	pthread_cond_init(barrier->release_threads, NULL);
	pthread_cond_init(barrier->next_bar, NULL);
	pthread_mutex_init(barrier->mutex, NULL);

	return EXIT_SUCCESS;
}


int barrier_wait(barrier_t *barrier)
{
	if (!barrier)
		return EINVAL;

	pthread_mutex_lock(barrier->mutex);
	while (barrier->curr_count != barrier->init_count)
		pthread_cond_wait(barrier->next_bar, barrier->mutex);
	(barrier->left)--;
	while (barrier->left != 0)
		pthread_cond_wait(barrier->release_threads, barrier->mutex);
	(barrier->curr_count)--;
	if (barrier->curr_count == barrier->init_count-1)
	{
		pthread_cond_broadcast(barrier->release_threads);
		pthread_mutex_unlock(barrier->mutex);
		return PTHREAD_BARRIER_SERIAL_THREAD;
	}
	if (barrier->curr_count == 0)
	{
		barrier->left = barrier->curr_count = barrier->init_count;
		pthread_cond_broadcast(barrier->next_bar);
	}
	pthread_mutex_unlock(barrier->mutex);

	return EXIT_SUCCESS;
}


int barrier_destroy(barrier_t *barrier)
{
	if (!barrier)
		return EINVAL;

	pthread_cond_destroy(barrier->release_threads);
	pthread_cond_destroy(barrier->next_bar);
	pthread_mutex_destroy(barrier->mutex);

	free(barrier->release_threads);
	free(barrier->next_bar);
	free(barrier->mutex);

	barrier->release_threads = NULL;
	barrier->next_bar        = NULL;
	barrier->mutex           = NULL;

	return EXIT_SUCCESS;
}

