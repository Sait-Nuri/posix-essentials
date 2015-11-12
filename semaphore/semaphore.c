/*
 *  semaphore.c
 *  
 *
 *  Created by Mij <mij@bitchx.it> on 12/03/05.
 *  Original source file available at http://mij.oltrelinux.com/devel/unixprg/
 *
 */


#define _POSIX_SOURCE
#include <stdio.h>
/* sleep() */
#include <errno.h>
#include <unistd.h>
/* abort() and random stuff */
#include <stdlib.h>
/* time() */
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>


/* this skips program termination when receiving signals */
void signal_handler(int type);

/*
 * thread A is synchronous. When it needs to enter its
 * critical section, it can't do anything other than waiting.
 */
void *thread_a(void *);

/*
 * thread B is asynchronous. When it tries to enter its
 * critical section, it switches back to other tasks if
 * it hasn't this availability.
 */
void *thread_b(void *);


/* the semaphore */
sem_t mysem;

int main(int argc, char *argv[])
{
    pthread_t mytr_a, mytr_b;
    int ret;
    
    
    srand(time(NULL));
    signal(SIGHUP, signal_handler);
    signal(SIGUSR1, signal_handler);

    /*
     * creating the unnamed, local semaphore, and initialize it with
     * value 1 (max concurrency 1)
     */
    ret = sem_init(&mysem, 0, 1);
    if (ret != 0) {
        /* error. errno has been set */
        perror("Unable to initialize the semaphore");
        abort();
    }
    
    /* creating the first thread (A) */
    ret = pthread_create(&mytr_a, NULL, thread_a, NULL);
    if (ret != 0) {
        perror("Unable to create thread");
        abort();
    }

    /* creating the second thread (B) */
    ret = pthread_create(&mytr_b, NULL, thread_b, NULL);
    if (ret != 0) {
        perror("Unable to create thread");
        abort();
    }
    
    /* waiting for thread_a to finish */
    ret = pthread_join(mytr_a, (void *)NULL);
    if (ret != 0) {
        perror("Error in pthread_join");
        abort();
    }

    /* waiting for thread_b to finish */
    ret = pthread_join(mytr_b, NULL);
    if (ret != 0) {
        perror("Error in pthread_join");
        abort();
    }

    return 0;
}


void *thread_a(void *x)
{
    unsigned int i, num;
    int ret;
    
    
    printf(" -- thread A -- starting\n");
    num = ((unsigned int)rand() % 40);
    
    /* this does (do_normal_stuff, do_critical_stuff) n times */
    for (i = 0; i < num; i++) {
        /* do normal stuff */
        sleep(3 + (rand() % 5));
        
        /* need to enter critical section */
        printf(" -- thread A -- waiting to enter critical section\n");
        /* looping until the lock is acquired */
        do {
            ret = sem_wait(&mysem);
            if (ret != 0) {
                /* the lock wasn't acquired */
                if (errno != EINVAL) {
                    perror(" -- thread A -- Error in sem_wait. terminating -> ");
                    pthread_exit(NULL);
                } else {
                    /* sem_wait() has been interrupted by a signal: looping again */
                    printf(" -- thread A -- sem_wait interrupted. Trying again for the lock...\n");
                }
            }
        } while (ret != 0);
        printf(" -- thread A -- lock acquired. Enter critical section\n");
        
        /* CRITICAL SECTION */
        sleep(rand() % 2);
        
        /* done, now unlocking the semaphore */
        printf(" -- thread A -- leaving critical section\n");
        ret = sem_post(&mysem);
        if (ret != 0) {
            perror(" -- thread A -- Error in sem_post");
            pthread_exit(NULL);
        }
    }
    
    printf(" -- thread A -- closing up\n");

    pthread_exit(NULL);
}


void *thread_b(void *x)
{
    unsigned int i, num;
    int ret;
    
    
    printf(" -- thread B -- starting\n");
    num = ((unsigned int)rand() % 100);
    
    /* this does (do_normal_stuff, do_critical_stuff) n times */
    for (i = 0; i < num; i++) {
        /* do normal stuff */
        sleep(3 + (rand() % 5));
        
        /* wants to enter the critical section */
        ret = sem_trywait(&mysem);
        if (ret != 0) {
            /* either an error happened, or the semaphore is locked */
            if (errno != EAGAIN) {
                /* an event different from "the semaphore was locked" happened */
                perror(" -- thread B -- error in sem_trywait. terminating -> ");
                pthread_exit(NULL);
            }
            
            printf(" -- thread B -- cannot enter critical section: semaphore locked\n");
        } else {
            /* CRITICAL SECTION */
            printf(" -- thread B -- enter critical section\n");

            sleep(rand() % 10);
            
            /* done, now unlocking the semaphore */
            printf(" -- thread B -- leaving critical section\n");
            sem_post(&mysem);
        }
    }
    
    printf(" -- thread B -- closing up\n");
    
    /* joining main() */
    pthread_exit(NULL);
}


void signal_handler(int type)
{
    /* do nothing */
    printf("process got signal %d\n", type);
    return;
}
