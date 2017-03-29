/* pi.c
 * ----
 * Multithreaded pi calculation using POSIX threads
 * and dynamic scheduling.
 *
 * Developed by George Z. Zachos
 * based on pi_serial.c by VVD.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

#define N 500000000   /* Intervals */


typedef struct timeval timeval_t;


/* Global declarations and initializations */
double		W = 1.0 / ((double) N), W2, pi = 0.0;
int		chunksize, /* K */
		taskid,    /* the next task id to execute */
		ntasks;    /* total number of tasks */
pthread_mutex_t	resmut = PTHREAD_MUTEX_INITIALIZER,
		tskmut = PTHREAD_MUTEX_INITIALIZER;


/* Function prototypes */
void taskexecute(int taskid);
void *thrfunc(void *arg);


int main(int argc, char **argv)
{
	int        i,
	           nthr;   /* number of threads */
	pthread_t *tid;
	timeval_t  tv1, tv2;
	double     elapsed_time;

	/* Number of threads and chunk size are provided
	 * as command line arguments.
	 */
	if (argc < 3)
	{
		fprintf(stderr, "Invalid number of command-line arguments!\n");
		exit(EXIT_FAILURE);
	}

	nthr      = atoi(argv[1]);
	chunksize = atoi(argv[2]);

	if (nthr < 1)
	{
		fprintf(stderr, "Invalid number of threads!\n");
		exit(EXIT_FAILURE);
	}

	if (chunksize < 1)
	{
		fprintf(stderr, "Chunk size cannot be less than 1!\n");
		exit(EXIT_FAILURE);
	}

	if (N % chunksize != 0)
	{
		fprintf(stderr, "Chunk size must evenly divide the number of intervals!\n");
		exit(EXIT_FAILURE);
	}

	ntasks = N/chunksize;
	W2     = W*W;   /* Save some calculations */
	W      = 4*W;

	tid = (pthread_t *) malloc(nthr * sizeof(pthread_t));
	if (!tid)
	{
		perror("malloc");
		exit(errno);
	}

	/* Start timing */
	gettimeofday(&tv1, NULL);
	for (i = 0; i < nthr; i++)
		pthread_create(tid+i, NULL, thrfunc, NULL);
	for (i = 0; i < nthr; i++)
		pthread_join(tid[i], NULL);
	/* End timing */
	gettimeofday(&tv2, NULL);

	elapsed_time =	(tv2.tv_sec - tv1.tv_sec) +
			(tv2.tv_usec - tv1.tv_usec)*1.0E-6;

	printf("nthr = %d\tchunk = %d\tpi = %.10lf\ttime: %lf sec.\n", nthr, chunksize, pi, elapsed_time);

	free(tid);
	return EXIT_SUCCESS;
}


void taskexecute(int taskid)
{
	int    i;
	double tmp_sum = 0.0;

	for (i = taskid*chunksize; i < (taskid+1)*chunksize; i++)
		tmp_sum += W / (1.0 + (0.5 + (double) i)*(0.5 + (double) i)*W2);

	pthread_mutex_lock(&resmut);
	pi += tmp_sum;
	pthread_mutex_unlock(&resmut);
}


void *thrfunc(void *arg)
{
	int t;

	while (1)
	{
		pthread_mutex_lock(&tskmut);
		t = taskid++;
		pthread_mutex_unlock(&tskmut);
		if (t >= ntasks)
			break;
		taskexecute(t);
	}
	return NULL;
}
