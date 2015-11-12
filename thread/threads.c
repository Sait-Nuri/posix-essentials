/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *
 ********************************************************
 * simple_threads.c
 *
 * Simple multi-threaded example.
 * Creates two threads. While doing different things, they
 * both access and modify variables with global scope.
 * Those vars have been tought to be modified this way, so
 * this is *not* an example of a thread side effect. If each
 * thread'd been accessing the same variable, they could create
 * such kind of problems.
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

void do_one_thing(int *);           /* first function to thread */
void do_another_thing(int *);       /* second function to thread */
void do_wrap_up(int, int);          /* after joining threads... */

int r1 = 0, r2 = 0;

extern int
main(void)
{
   /* ids for the first and the second thread */
  pthread_t thread1, thread2;
 
   /* creating the first thread. retval != 0 means troubles */
  if (pthread_create(&thread1, 
         NULL,
         (void *) do_one_thing,
         (void *) &r1) != 0)
    perror("pthread_create"), exit(1); 
  
   /* creating the first thread. retval != 0 means troubles.
    * its argument is passed with a void * casting as requested
    * by pthread_create. The actual function expects an int. */
  if (pthread_create(&thread2, 
         NULL, 
         (void *) do_another_thing,
         (void *) &r2) != 0)
    perror("pthread_create"), exit(1); 
  
   /* waiting for the first thread to terminate.
    * Thread's return(/exit) value gets discarded. */
  if (pthread_join(thread1, NULL) != 0)
    perror("pthread_join"),exit(1);
 
   /* waiting for the second thread */
  if (pthread_join(thread2, NULL) != 0)
    perror("pthread_join"),exit(1);

  do_wrap_up(r1, r2);

  return 0; 
}

void do_one_thing(int *pnum_times)
{
  int i, j, x;
  
  for (i = 0;  i < 4; i++) {
    printf("doing one thing\n"); 
    for (j = 0; j < 10000; j++) x = x + i;
    (*pnum_times)++;
  }

}

void do_another_thing(int *pnum_times)
{
  int i, j, x;
  
  for (i = 0;  i < 4; i++) {
    printf("doing another \n"); 
    for (j = 0; j < 10000; j++) x = x + i;
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