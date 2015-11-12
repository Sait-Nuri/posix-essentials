/*
 *  msgqattrs.c
 *  
 *  creates a posix message queue requesting custom attributes,
 *  and displays what attributes are taken into account and what
 *  are not, while both creating and while setting at a later time.
 *
 *
 *  Created by Mij <mij@bitchx.it> on 31/08/05.
 *  Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 *
 */


#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
/* exit() */
#include <stdlib.h>


#define MSGQOBJ_NAME        "/fooasd1431"
#define MAX_QMSG_SIZE       30


int main(int argc, char *argv[]) {
    mqd_t myqueue;
    struct mq_attr wanted_attrs, actual_attrs;
    
        
    /* filling the attribute structure */
    wanted_attrs.mq_flags = 0;                    /* no exceptional behavior (just O_NONBLOCK currently available)  */
    wanted_attrs.mq_maxmsg = 100;                 /* room for at most 100 messages in the queue */
    wanted_attrs.mq_msgsize = MAX_QMSG_SIZE;      /* maximum size of a message */
    wanted_attrs.mq_curmsgs = 123;                /* this (current number of messages) will be ignored */
    
    /* mq_open() for creating a new queue (using default attributes) */
    myqueue = mq_open(MSGQOBJ_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, &wanted_attrs);
    if (myqueue == (mqd_t)-1) {
        perror("In mq_open()");
        exit(1);
    }

    printf("Message queue created.\n");
    
    /* getting queue attributes after creation          --  mq_getattr()    */
    mq_getattr(myqueue, &actual_attrs);
    printf("Attributes right after queue creation:\n\t- non blocking flag: %d\n\t- maximum number of messages: %ld\n\t- maximum size of a message: %ld\n\t- current number of messages: %ld\n", (actual_attrs.mq_flags == 0 ? 0 : 1), actual_attrs.mq_maxmsg, actual_attrs.mq_msgsize, actual_attrs.mq_curmsgs);
    
    /* building the structure again for modifying the existent queue */
    wanted_attrs.mq_flags = O_NONBLOCK;
    wanted_attrs.mq_maxmsg = 350;               /* this will be ignored by mq_setattr() */
    wanted_attrs.mq_msgsize = MAX_QMSG_SIZE;    /* this will be ignored by mq_setattr() */
    wanted_attrs.mq_curmsgs = 123;              /* this will be ignored by mq_setattr() */
    
    /* trying to later set different attributes on the queue        --  mq_setattr()    */
    mq_setattr(myqueue, &wanted_attrs, NULL);
    
    /* getting queue attributes after creation */
    mq_getattr(myqueue, &actual_attrs);
    printf("Attributes after setattr():\n\t- non blocking flag: %d\n\t- maximum number of messages: %ld\n\t- maximum size of a message: %ld\n\t- current number of messages: %ld\n", (actual_attrs.mq_flags == 0 ? 0 : 1), actual_attrs.mq_maxmsg, actual_attrs.mq_msgsize, actual_attrs.mq_curmsgs);
    
    /* removing the queue from the system       --  mq_unlink() */
    mq_unlink(MSGQOBJ_NAME);
    
    
    return 0;
}

