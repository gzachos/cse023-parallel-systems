typedef struct barrier_s {
	unsigned int     init_count;
	unsigned int     arrived;
	unsigned int     left;
	pthread_cond_t  *release_threads;
	pthread_cond_t  *next_bar;
	pthread_mutex_t *mutex;
} barrier_t;
