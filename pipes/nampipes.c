/*
 *  nampipes.c
 *  
 *  simply opens a pre-created named pipe (a "fifo") and reads
 *  stuff from it as soon as there's something available.
 *
 *
 *  Created by Mij <mij@bitchx.it> on 02/02/05.
 *  Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define MAX_LINE_LENGTH


int main(int argc, char *argv[]) {
    int pipe;
    char ch;
    

    /* we expect a named pipe has been created
     * in /tmp/named_pipe . The command
     *   $ mkfifo /tmp/named_pipe
     * is a friend to get to this point
     */
    pipe = open("/tmp/named_pipe", O_RDONLY);
    if (pipe < 0) {
        perror("Opening pipe");
        exit(1);
    }
        
    /* preparing to read from the pipe... */
    printf("Waiting data from the pipe... \n");

    /* reading one char a time from the pipe */
    while (1) {
        if (read(pipe, &ch, 1) < 0) {
            perror("Read the pipe");
            exit(2);
        }
        
        printf("%c", ch);
    }
    
    /* leaving the pipe */
    close(pipe);
    
    return 0;
}
