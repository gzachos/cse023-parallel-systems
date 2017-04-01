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
	barrier->init_count = count;
	barrier->left = barrier->arrived = 0;

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
	unsigned int init_count;
	int          retval = EXIT_SUCCESS;

	if (!barrier)
		return EINVAL;

	pthread_mutex_lock(barrier->mutex);
	init_count = barrier->init_count;
	while (barrier->left > 0)
		pthread_cond_wait(barrier->next_bar, barrier->mutex);
	++(barrier->arrived);
	while (barrier->arrived != init_count)
		pthread_cond_wait(barrier->release_threads, barrier->mutex);
	if ((barrier->left)++ == 0)
	{
		retval = PTHREAD_BARRIER_SERIAL_THREAD;
		pthread_cond_broadcast(barrier->release_threads);
	}
	if (barrier->left == init_count)
	{
		barrier->left = barrier->arrived = 0;
		pthread_cond_broadcast(barrier->next_bar);
	}
	pthread_mutex_unlock(barrier->mutex);

	return retval;
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

