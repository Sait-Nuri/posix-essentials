/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *
 ********************************************************
 * simple_mutex.c
 *
 * Simple multi-threaded example with a mutex lock.
 * Does the same as the example above (see the link below)
 * but with mutual exclusion. Any time a thread starts, it
 * request the thread lock. Just one thread is executing
 * anytime. The others must wait it to unlock before proceeding.
 * quickly comment out by Mij <mij@bitchx.it> for
 * http://mij.oltrelinux.com/devel/unixprg/
 *
 * (also modified a bit on the code itself for clarity and
 * expressiveness purposes)
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

void do_one_thing(int *);       /* first thread */
void do_another_thing(int *);       /* second thread */
void do_wrap_up(int, int);

int r1 = 0, r2 = 0, r3 = 0;
pthread_mutex_t r3_mutex=PTHREAD_MUTEX_INITIALIZER; /* for mutex locking */

extern int
main(int argc, char **argv)
{
   /* thread ids */
  pthread_t thread1, thread2;

  if (argc > 1) 
  	r3 = atoi(argv[1]);

   /* creating the first thread */
  if (pthread_create(&thread1, 
		 NULL,
		 (void *) do_one_thing,
		 (void *) &r1) != 0)
	perror("pthread_create"),exit(1); 

   /* creating the second thread */
  if (pthread_create(&thread2, 
		 NULL, 
		 (void *) do_another_thing,
		 (void *) &r2) != 0)
	perror("pthread_create"),exit(1); 
  
   /* expecting the first thread to terminate */
  if (pthread_join(thread1, NULL) != 0)
	perror("pthread_join"), exit(1);
  
   /* expecting the second thread to terminate */
  if (pthread_join(thread2, NULL) != 0)
	perror("pthread_join"), exit(1);

  do_wrap_up(r1, r2);

  return 0; 
}

void do_one_thing(int *pnum_times)
{
  int i, j, x;
  pthread_t ti;

  ti = pthread_self();      /* which's our id? */
  pthread_mutex_lock(&r3_mutex);
   /* this is the segment containing sensitive operations.
    * So we need to keep it alone from concurrency for safeness. */
  if(r3 > 0) {
	x = r3;
	r3--;
  } else {
	x = 1;
  }
   /* sensitive code end */
  pthread_mutex_unlock(&r3_mutex); 

  for (i = 0;  i < 4; i++) {
    printf("doing one thing\n"); 
      for (j = 0; j < 10000; j++) x = x + i;
      printf("thread %d: got x = %d\n", (int)ti, x);
    (*pnum_times)++;
  }

}

void do_another_thing(int *pnum_times)
{
  int i, j, x;
  pthread_t ti;
    
  ti = pthread_self();
  pthread_mutex_lock(&r3_mutex);
  if(r3 > 0) {
        x = r3;
        r3--;
  } else {
        x = 1;
  }
  pthread_mutex_unlock(&r3_mutex);

  for (i = 0;  i < 4; i++) {
    printf("doing another \n"); 
    for (j = 0; j < 10000; j++) x = x + i;
    printf("thread %d: got x = %d\n", (int)ti, x);
    (*pnum_times)++;
  }

}

void do_wrap_up(int one_times, int another_times)
{
  int total;

  total = one_times + another_times;
  printf("All done, one thing %d, another %d for a total of %d\n",
	one_times, another_times, total);
}
