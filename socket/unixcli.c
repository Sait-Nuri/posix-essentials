/*
 * unixcli.c
 *
 * connects to an UNIX domain socket, sends a message to it, and disconnects.
 *
 *
 * Created by Mij <mij@bitchx.it> on 18/12/05.
 * Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 */

#include <stdio.h>
/* socket(), bind() */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
/* write(), close() */
#include <unistd.h>
/* strlen() */
#include <string.h>
/* exit() */
#include <stdlib.h>

/* maximum size available for an incoming message payload */
#define MAX_MSGLEN      100
/* path of the UNIX domain socket in filesystem */
#define SERVERSOCK_PATH     "/tmp/mytmpunixsock"

int main() {
    int sock, err, i = 0;
    char ch, messagebuf[MAX_MSGLEN];
    struct sockaddr_un servaddr;


    /* create a UNIX domain, connectionful socket */
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("In socket()");
        exit(1);
    }

    /* connect the socket to the server socket described by "servaddr" */
    servaddr.sun_family = AF_UNIX;
    sprintf(servaddr.sun_path, SERVERSOCK_PATH);

    err = connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (err) {
        perror("In connect()");
        exit(1);
    }

    /* write a message to the server */
    err = write(sock, "Hello server.\n", strlen("Hello server.\n"));
    if (err < 0) {
        perror("In write()");
        exit(1);
    }

    printf("Message sent:\n\t@length: %d bytes\n\t@content: %s\n", err, "Hello server.\n");

    /* receive the response from the server */
    do {
        err = read(sock, &ch, 1);       /* read one byte from the socket */
        if (err <= 0) {
            printf("Premature end-of-file (0) or read() error (<0)? %d\n", err);
            break;
        }
        messagebuf[i++] = ch;
    } while (ch != '\n');
    messagebuf[i] = '\0';
   
    /* close the socket */
    close(sock);
    
    printf("Response received:\n\t@length: %lu bytes\n\t@content: %s\n", strlen(messagebuf), messagebuf);

    return 0;
}

