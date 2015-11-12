/*
 * udpserv.c
 *
 * listens on an UDP port, accept one message and displays its content and
 * who's the sender
 *
 *
 * Created by Mij <mij@bitchx.it> on 18/12/05.
 * Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 */

#include <stdio.h>
/* socket(), bind(), recvfrom() */
#include <sys/types.h>
#include <sys/socket.h>
/* sockaddr_in */
#include <netinet/in.h>
/* inet_addr() */
#include <arpa/inet.h>
/* memset() */
#include <string.h>
/* close() */
#include <unistd.h>
/* exit() */
#include <stdlib.h>

/* maximum size available for an incoming message payload */
#define MAX_MSGLEN      100

int main() {
    int sock, err;
    char messagebuf[MAX_MSGLEN+1];
    struct sockaddr_in saddr, fromaddr;
    int fromaddr_len = sizeof(fromaddr);


    /* create a INET-domain, disconnected (datagram) socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("In socket()");
        exit(1);
    }

    /* bind the following attributes to the socket: */
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");     /* localhost address */
    saddr.sin_port = htons(61321);          /* use port 61321 (with correct network byte-order */
    memset(&saddr.sin_zero, 0, sizeof(saddr.sin_zero)); /* always zero-fill this field! */
    
    err = bind(sock, (struct sockaddr *)&saddr, sizeof(saddr));
    if (err) {
        perror("In bind()");
        exit(1);
    }
   
    /* receive a message in "messagebuf" at most MAX_MSGLEN bytes long (1 is
     * spared for the trailing '\0'; store sender info in "fromaddr" */
    err = recvfrom(sock, messagebuf, MAX_MSGLEN-1, 0, (struct sockaddr *)&fromaddr, (socklen_t *)&fromaddr_len);
    if (err <= 0) {     /* otherwise, "err" tells how many bytes have been written in "messagebuf" */
        perror("in recvfrom()");
        exit(1);
    }
    messagebuf[err] = '\0';        /* NULL-terminator */

    /* close the socket */
    close(sock);
    
    /* displaying message length and content */
    printf("Message:\n\t@bytes: %d\n\t@payload: %s\n", err, messagebuf);
    /* displaying sender info: */
    printf("Sender:\n\t@address: %s\n\t@port: %d\n", inet_ntoa(fromaddr.sin_addr), ntohs(fromaddr.sin_port));

    return 0;
}

