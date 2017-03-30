/* test.c
 * ------
 * Test POSIX threads' and custom barrier.
 * 
 * Developed by George Z. Zachos
 */

// #define USE_PTHREAD_BAR

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#ifndef USE_PTHREAD_BAR
	#include "gz_barrier.h"
#endif

#define NTHR 5


/* Global definitions */
typedef struct thrarg_s {
	int tid;
} thrarg_t;

/* Function prototypes */
void *thrfunc(void *arg);

#ifdef USE_PTHREAD_BAR
pthread_barrier_t bar;
#else
barrier_t bar;
#endif

int main(void)
{
	int i;
	pthread_t tid[NTHR];
	thrarg_t arg[NTHR];

#ifdef USE_PTHREAD_BAR
	pthread_barrier_init(&bar, NULL, NTHR);
#else
	barrier_init(&bar, NTHR);
#endif

	for (i = 0; i < NTHR; i++)
	{
		(arg+i)->tid = i;
		pthread_create(tid+i, NULL, thrfunc, ((void *)(arg+i)));
	}
	for (i = 0; i < NTHR; i++)
		pthread_join(tid[i], NULL);

#ifdef USE_PTHREAD_BAR
	pthread_barrier_destroy(&bar);
#else
	barrier_destroy(&bar);
#endif

	return EXIT_SUCCESS;
}


void *thrfunc(void *arg)
{
	int tid = (int) (((thrarg_t *)arg)->tid);

	printf("thread %d @ region 1\n", tid);
	if (tid == 2)
	{
		sleep(2);
		printf("\n\nthread 2 just before calling barrier_wait()\n\n");
	}
#ifdef USE_PTHREAD_BAR
	pthread_barrier_wait(&bar);
#else
	barrier_wait(&bar);
#endif
	printf("thread %d @ region 2\n", tid);

	if (tid == 4)
	{
		sleep(2);
		printf("\n\nthread 4 just before calling barrier_wait()\n\n");
	}
#ifdef USE_PTHREAD_BAR
	pthread_barrier_wait(&bar);
#else
	barrier_wait(&bar);
#endif
	printf("thread %d @ region 3\n", tid);

#if 0
#ifdef USE_PTHREAD_BAR
	pthread_barrier_wait(&bar);
	pthread_barrier_wait(&bar);
#else
	barrier_wait(&bar);
	barrier_wait(&bar);
#endif
	printf("thread %d @ region 4\n", tid);
#endif

	return NULL;
}
