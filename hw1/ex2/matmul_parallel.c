/* matmul_parallel.c
 * -----------------
 * Parallel program for matrix-matrix product.
 *
 * Developed by George Z. Zachos
 * based on matmul_serial.c by VVD.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

#define N 1024 /* Array dimension */


/* Global declarations and definitions */
int A[N][N], B[N][N], C[N][N], chunk, sthreads;

typedef struct timeval timeval_t;
typedef struct thrarg_s {
	int tid;
} thrarg_t;


/* Function prototypes */
int   readmat(char *fname, int *mat, int n), 
      writemat(char *fname, int *mat, int n);
void *thrfunc(void *arg);


int main(int argc, char **argv)
{
	int        i,
	           nthr;   /* number of threads */
	pthread_t *tid;
	timeval_t  tv1, tv2;
	thrarg_t  *arg;
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

	chunk    = N / nthr; /* The # of iterations dispatched to one thread */
	sthreads = N % nthr; /* Special threads execute (chunk+1) iterations */

	/* Read matrices from files: "A_file", "B_file" */
	if (readmat("Amat1024", (int *) A, N) < 0) 
		exit( 1 + printf("file problem\n") );
	if (readmat("Bmat1024", (int *) B, N) < 0) 
		exit( 1 + printf("file problem\n") );

	tid = (pthread_t *) malloc(nthr * sizeof(pthread_t));
	arg = (thrarg_t *)  malloc(nthr * sizeof(thrarg_t));
	if (!tid || !arg)
	{
		perror("malloc");
		exit(errno);
	}

	/* Start timing */
	gettimeofday(&tv1, NULL);
	for (i = 0; i < nthr; i++)
	{
		(arg+i)->tid = i;
		pthread_create(tid+i, NULL, thrfunc, ((void *)(arg+i)));
	}
	for (i = 0; i < nthr; i++)
		pthread_join(tid[i], NULL);
	/* End timing */
	gettimeofday(&tv2, NULL);

	elapsed_time =  (tv2.tv_sec - tv1.tv_sec) +
	                (tv2.tv_usec - tv1.tv_usec)*1.0E-6;

	printf("nthr = %d\ttime: %lf sec.\n", nthr, elapsed_time);

	/* Save result in "Cmat1024" */
	writemat("Cmat1024", (int *) C, N);

	free(tid);
	free(arg);
	return (EXIT_SUCCESS);
}


void *thrfunc(void *arg)
{
	int i, j, k, sum, lb, ub,
	    tid = ((thrarg_t *) arg)->tid;

	if (sthreads > 0)
	{
		if (tid < sthreads)
		{
			lb = tid * (chunk+1);
			ub = lb + (chunk+1);
		}
		else
		{
			lb = sthreads*(chunk+1) + (tid-sthreads)*chunk;
			ub = lb + chunk;
		}
	}
	else
	{
		lb = tid * chunk;
		ub = lb + chunk;
	}

	for (i = lb; i < ub; i++)
		for (j = 0; j < N; j++)
		{
			for (k = sum = 0; k < N; k++)
				sum += A[i][k]*B[k][j];
			C[i][j] = sum;
		};

	return NULL;
}


/* Utilities to read & write matrices from/to files
 * VVD
 */

#define _mat(i,j) (mat[(i)*n + (j)])


int readmat(char *fname, int *mat, int n)
{
	FILE *fp;
	int  i, j;
	
	if ((fp = fopen(fname, "r")) == NULL)
		return (-1);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (fscanf(fp, "%d", &_mat(i,j)) == EOF)
			{
				fclose(fp);
				return (-1); 
			};
	fclose(fp);
	return (EXIT_SUCCESS);
}


int writemat(char *fname, int *mat, int n)
{
	FILE *fp;
	int  i, j;
	
	if ((fp = fopen(fname, "w")) == NULL)
		return (-1);
	for (i = 0; i < n; i++, fprintf(fp, "\n"))
		for (j = 0; j < n; j++)
			fprintf(fp, " %d", _mat(i, j));
	fclose(fp);
	return (EXIT_SUCCESS);
}
