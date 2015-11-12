/*
 *  shm_anon_bsd.c
 *  
 *  Anonymous shared memory via BSD's MAP_ANON.
 *  Create a semaphore, create an anonymous memory segment with the MAP_ANON
 *  BSD flag and loop updating the segment content (increment casually) with
 *  short intervals.
 *
 *
 *  Created by Mij <mij@bitchx.it> on 29/08/05.
 *  Original source file available at http://mij.oltrelinux.com/devel/unixprg/
 *
 */

#include <stdio.h>
/* for shm_* and mmap() */
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
/* for getpid() */
#include <unistd.h>
/* exit() */
#include <stdlib.h>
/* for sem_* functions */
#include <sys/stat.h>
#include <semaphore.h>
/* for seeding time() */
#include <time.h>

/* name of the semaphore */
#define     SEMOBJ_NAME         "/semshm"
/* maximum number of seconds to sleep between each loop operation */
#define     MAX_SLEEP_SECS      3
/* maximum value to increment the counter by */
#define     MAX_INC_VALUE       10 


int main(int argc, char *argv[]) {
    int shmfd;
    int shared_seg_size = 2 * sizeof(int);
    int *shared_values;     /* this will be a (shared) array of 2 elements */
    sem_t *sem_shmsegment;  /* semaphore controlling access to the shared segment */
    pid_t mypid;
    
    
    /* getting a new semaphore for the shared segment       -- sem_open()   */
    sem_shmsegment = sem_open(SEMOBJ_NAME, O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, 1);
    if ((int *)sem_shmsegment == SEM_FAILED) {
        perror("In sem_open()");
        exit(1);
    }
    /* requesting the semaphore not to be held when completely unreferenced */
    sem_unlink(SEMOBJ_NAME);
    
    /* requesting the shared segment    --  mmap() */    
    shared_values = (int *)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (shared_values == (int*)-1) {
        perror("In mmap()");
        exit(1);
    }
    fprintf(stderr, "Shared memory segment allocated correctly (%d bytes) at %u.\n", shared_seg_size, (unsigned int*)shared_values);
    close(shmfd);


    /* dupping the process */
    if (! fork() )
        /* the child waits 2 seconds for better seeding srandom() */
        sleep(2);
    
    /* seeding the random number generator (% x for better seeding when child executes close) */
    srandom(time(NULL));

    /* getting my pid, and introducing myself */
    mypid = getpid();
    printf("My pid is %d\n", mypid);    

    /*
       main loop:
        - pause
        - print the old value
        - choose (and store) a random quantity
        - increment the segment by that
    */
    do {
        sleep(random() % (MAX_SLEEP_SECS+1));       /* pausing for at most MAX_SLEEP_SECS seconds */
        
        sem_wait(sem_shmsegment);
        /* entered the critical region */
        
        printf("process %d. Former value %d.", mypid, shared_values[0]);
        
        shared_values[1] = random() % (MAX_INC_VALUE+1);            /* choose a random value up to MAX_INC_VALUE */
        shared_values[0] += shared_values[1];   /* and increment the first cell by this value */
        
        printf(" Incrementing by %d.\n", shared_values[1]);

        /* leaving the critical region */
        sem_post(sem_shmsegment);
    } while (1);
    
    /* freeing the reference to the semaphore */
    sem_close(sem_shmsegment);

    
    return 0;
}
