/* pi_mpi.c
 * --------
 * Parallel pi calculation using MPI and self-scheduling.
 * (Master/Slave Model)
 *
 * Developed by George Z. Zachos
 * based on pi_serial.c by VVD.
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHUNKSIZE         10000
#define HANDSHAKE_MSG     (status.MPI_TAG == 1)
#define NO_PENDING_WORK   (status.MPI_TAG == 0)

enum stat_e {CALC, COMM, MIN_CALC, MIN_COMM, MAX_CALC, MAX_COMM};

int main(int argc, char **argv)
{
	int        myrank, nnodes, N, i, num_tasks,
	           taskid = 0, dispatched = 0, signaled = 0;
	double     W, result = 0.0, partial_res = 0.0,
	           t[9], time[2], stats[6];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

	if (myrank == 0)
	{
		printf("Enter the number of tasks: ");
		fflush(stdout);
		scanf("%d", &num_tasks);
		printf("\n");
		N = num_tasks * CHUNKSIZE;
		if (num_tasks <= 0)
		{
			fprintf(stderr, "The number of tasks should be a positive integer\n");
			MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}
	t[0] = MPI_Wtime();
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	t[1] = MPI_Wtime();
	time[COMM] += t[1] - t[0];

	if (myrank != 0)
	{
		while (1)
		{
			/* The first time here, every MPI slave process sends a
			 * handshake to the master process. From that point on
			 * it receives a reply message and if work is dispatched
			 * it serves the request and replies to master with a
			 * message containing the partial result calculated.
			 */
			t[2] = MPI_Wtime();
			MPI_Send(&result, 1, MPI_DOUBLE, 0, (result == 0.0) ? 1 : 0, MPI_COMM_WORLD);
			MPI_Recv(&taskid, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			t[3] = MPI_Wtime();
			time[COMM] += t[3] - t[2];
			if (NO_PENDING_WORK)
				break;
			result = 0.0;
			W = 1.0 / N;
			for (i = taskid * CHUNKSIZE; i < (taskid+1) * CHUNKSIZE; i++)
				result += 4*W / (1 + (i+0.5)*(i+0.5)*W*W);
		}
	}
	else
	{
		while (1)
		{
		 	/* The master MPI process continuously receives new
			 * messages. In case of handshake messages it dispatches
			 * a chunk of work to the slave and in opposite case
			 * it updates 'result' and then it either dispatches
			 * work or signals the slave process to terminate.
			 */
			t[2] = MPI_Wtime();
			MPI_Recv(&partial_res, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			t[3] = MPI_Wtime();
			time[COMM] += t[3] - t[2];
			if (!HANDSHAKE_MSG)
				result += partial_res;
			if (dispatched == num_tasks) /* Signal slave to quit */
			{
				t[4] = MPI_Wtime();
				MPI_Send(&taskid, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
				t[5] = MPI_Wtime();
				time[COMM] += t[5] - t[4];
				if (++signaled == nnodes-1) /* nnodes-1: All processes but the master */
					break;
				continue;
			}
			/* Dispatch a chunk of work */
			taskid = dispatched++;
			t[6] = MPI_Wtime();
			MPI_Send(&taskid, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
			t[7] = MPI_Wtime();
			time[COMM] += t[7] - t[6];
		}
		printf("pi = %.10lf\n", result);
	}

	t[8] = MPI_Wtime();
	time[CALC] = t[8] - t[0] - time[COMM];
	MPI_Reduce(time, stats,            2, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(time, stats + MIN_CALC, 2, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce(time, stats + MAX_CALC, 2, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (myrank == 0)
	{
		stats[CALC] /= nnodes;
		stats[COMM] /= nnodes;

		printf("Time:\n");
		printf("   Total   : %2lf\n", stats[CALC] + stats[COMM]);
		printf("   Avg Calc: %2lf\n", stats[CALC]);
		printf("   Max Calc: %2lf\n", stats[MAX_CALC]);
		printf("   Min Calc: %2lf\n", stats[MIN_CALC]);
		printf("   Avg Comm: %2lf\n", stats[COMM]);
		printf("   Max Comm: %2lf\n", stats[MAX_COMM]);
		printf("   Min Comm: %2lf\n", stats[MIN_COMM]);
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}
