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
#include <limits.h>
#include <errno.h>
#include <sys/time.h>
#ifndef USE_PTHREAD_BAR
	#include "gz_barrier.h"
#endif

#define ITERATIONS	100


/* Global definitions */
typedef struct timeval timeval_t;

/* Function prototypes */
void *thrfunc(void *arg);

#ifdef USE_PTHREAD_BAR
pthread_barrier_t bar;
#else
barrier_t bar;
#endif

int main(int argc, char **argv)
{
	int        i,
	           nthr;   /* number of threads */
	pthread_t *tid;
	timeval_t  tv1, tv2;
	double     elapsed_time;

	/* The number of threads is provided as a command line argument */
	if (argc < 2)
	{
		fprintf(stderr, "Invalid number of command-line arguments!\n");
		exit(EXIT_FAILURE);
	}

	nthr = atoi(argv[1]);
	if (nthr < 1)
	{
		fprintf(stderr, "Invalid number of threads!\n");
		exit(EXIT_FAILURE);
	}

	tid = (pthread_t *) malloc(nthr * sizeof(pthread_t));
	if (!tid)
	{
		perror("malloc");
		exit(errno);
	}

#ifdef USE_PTHREAD_BAR
	pthread_barrier_init(&bar, NULL, nthr);
#else
	barrier_init(&bar, nthr);
#endif

	/* Start timing */
	gettimeofday(&tv1, NULL);
	for (i = 0; i < nthr; i++)
		pthread_create(tid+i, NULL, thrfunc, NULL);
	for (i = 0; i < nthr; i++)
		pthread_join(tid[i], NULL);
	/* End timing */
	gettimeofday(&tv2, NULL);

	elapsed_time =  (tv2.tv_sec - tv1.tv_sec) * 1000000 +
			(tv2.tv_usec - tv1.tv_usec);
	elapsed_time /= 1000;

#ifdef USE_PTHREAD_BAR
	pthread_barrier_destroy(&bar);
#else
	barrier_destroy(&bar);
#endif

#ifdef USE_PTHREAD_BAR
	printf("\nPTHREADS' Barrier: nthr = %d\ttime: %lf msec.\n",
		nthr, elapsed_time);
#else
	printf("\nGZachos' Barrier: nthr = %d\ttime: %lf msec.\n",
		nthr, elapsed_time);

#endif

	free(tid);
	return EXIT_SUCCESS;
}


void *thrfunc(void *arg)
{
	int i, j, a;

	for (i = 0; i < ITERATIONS; i++)
	{
		for (j = 0, a = 0; j < (INT_MAX >> 5); j++)
			a += j / (2 + a);

#ifdef USE_PTHREAD_BAR
		pthread_barrier_wait(&bar);
#else
		barrier_wait(&bar);
#endif
	}
	return NULL;
}
