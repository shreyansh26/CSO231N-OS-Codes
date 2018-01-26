/* usage - gcc -pthread POSIX-Pthread.c -o POSIX-Pthread */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int sum; /* this data is shared between the threads */
void *runner(void *param); /* threads call this function */

int main(int argc, char **argv) {
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr;  /* set of thread attributes */

	if (argc != 2) {
		fprintf(stderr, "usage: a.out <integer-value>\n");
		return -1;
	}
	if(atoi(argv[1]) < 0) {
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;
	}

	/* get the default attributes */
	pthread_attr_init(&attr);
	/* create the thread */
	pthread_create(&tid, &attr, runner, argv[1]);
	/* wait for the thread to exit */
	pthread_join(tid, NULL);

	printf("Sum = %d\n", sum);
	return 0;
}

/* The thread will begin control in this function */
void *runner(void *param) {
	int i, upper = atoi(param);
	sum = 0;
	for(int i=0; i<=upper; i++) {
		sum += i;
	}
	pthread_exit(0);
}