Selective Repeat ARQ Simulation

        *File : selective -
                repeat - sendr.c *This is the sender part of the Selective Repeat ARQ protocol

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>

#define SA struct sockaddr
#define MAXBUF 50
#define PORT 50000
#define STR "This is our data"
#define TIMEOUT 2

                         struct timeval value;

struct frame
{
    int sock, window, slast, srecent, threshold;
    struct head
    {
        int seq, ack, nak;
    } header;
    char data[MAXBUF];
};

struct frame f;

void handler(int sig)
{
    struct frame f;
    int ackno, seqno, nakno, n;

    switch (sig)
    {
    case SIGIO:
        n = recv(sock, &f, sizeof(f), 0);
        ackno = f.header.ack;
        nakno = f.header.nak;

        if ((nakno > -1) && (ackno == -1))
        { // NAK received
            printf("Received NAK = %d\n", nakno);
            f.header.seq = nakno;
            f.header.ack = -1;
            f.header.nak = -1;
            printf("Sending repeat frame %d\n", nakno);
            send(sock, &f, sizeof(f), 0);
            value.it_value.tv_sec = TIMEOUT;
            setitimer(ITIMER_REAL, &value, 0);
            sleep(1);
        }
        else
        {
            // ACK received
            printf("ACK %d received\n", ackno);
            slast = ackno;
        }
        break;

    case SIGALRM:
        printf("SIGALRM: Time out... going back to slast = %d\n", slast);
        srecent = slast;
        break;

    case SIGINT:
        close(sock);
        printf("\n\t..............Thanks to use this software..............\n");
        printf("\n\t..............Exiting..........\n");
        exit(0);
        break;
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_in recvr;
    int flags, noise;
    struct sigaction ioact, alrmact, intact;

    if (argc != 4)
    {
        printf("Usage: %s <IPaddress of the receiver machine> <send window> <channel noise threshold>\n", argv[0]);
        exit(-1);
    }

    window = atoi(argv[2]);
    threshold = atoi(argv[3]);

    bzero(&recvr, sizeof(recvr));
    recvr.sin_family = AF_INET;
    recvr.sin_port = htons(PORT);

    if (inet_aton(argv[1], &recvr.sin_addr) == 0)
    {
        printf("Address conversion error\n");
        exit(-2);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket error: ");
        exit(-3);
    }

    if (connect(sock, (SA *)&recvr, sizeof(recvr)) < 0)
    {
        printf("No channel could be established\n");
        exit(-4);
    }

    printf("Channel established\n");

    /* establish signal handler for SIGIO */
    ioact.sa_handler = handler;
    sigemptyset(&ioact.sa_mask);
    sigaddset(&ioact.sa_mask, SIGALRM);
    sigaction(SIGIO, &ioact, NULL);

    /* establish signal handler for SIGALRM */
    alrmact.sa_handler = handler;
    sigemptyset(&alrmact.sa_mask);
    sigaddset(&alrmact.sa_mask, SIGIO);
    sigaction(SIGALRM, &alrmact, NULL);

    /* prepare for async socket IO */
    fcntl(sock, F_SETOWN, getpid());
    fcntl(sock, F_SETFL, O_ASYNC);

    slast = srecent = 0;

    for (;;)
    {
        if (srecent > (slast + window - 1))
        {
            printf("Window exhausted waiting for timer to timeout\n");
            sleep(3);
        }
        else
        {
            noise = rand() % 10;

            if (noise < threshold)
            {
                f.header.seq = srecent;
                f.header.ack = -1;
                f.header.nak = -1;
                strcpy(f.data, STR);
                printf("Sending frame seq no = %d\n", f.header.seq);
                send(sock, &f, sizeof(f), 0);

                value.it_value.tv_sec = TIMEOUT;
                setitimer(ITIMER_REAL, &value, 0);
            }
            else
            {
                printf("Channel is too noisy... frame %d is lost\n", srecent);
            }
            srecent++;
            sleep(1);
        }
    }

    close(sock);
    return 0;
}

(Receiver Side Code)

// File : Go-back-N-recvr.c

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SA struct sockaddr
#define MAXBUF 50
#define PORT 50000
#define STR "This is our data"

    int rlast,
    rnext, nwindow;

struct head
{
    int seq, ack, nak;
};

struct frame
{
    struct head header;
    char data[MAXBUF];
};

void sendAck(int comm, int ack, int threshold)
{
    int noise, len;
    struct frame fr;
    noise = rand() % 10;

    if (noise < threshold)
    {
        fr.header.seq = -1;
        fr.header.ack = ack;
        fr.header.nak = -1;
        strcpy(fr.data, STR);
        printf("Sending ACK = %d frame.....\n", ack);
        send(comm, &fr, sizeof(fr), 0);
    }
    else
    {
        printf("Channel is too noisy! ACK is lost\n");
    }
}

void sendNak(int link, int nak, int threshold)
{
    int noise, len;
    struct frame fr;
    noise = rand() % 10;

    if (noise < threshold)
    {
        fr.header.seq = -1;
        fr.header.ack = -1;
        fr.header.nak = nak;
        strcpy(fr.data, STR);
        printf("Sending NAK %d\n", nak);
        send(link, &fr, sizeof(fr), 0);
    }
    else
    {
        printf("Channel is too noisy! NAK is lost\n");
    }
}

int main(int argc, char **argv)
{
    struct sockaddr_in this, sendr;
    int sock, comm;
    socklen_t len;
    int seqno, ackno = 0;
    struct frame f;
    int threshold;

    if (argc != 3)
    {
        printf("Usage: %s <noise threshold level> <Receive window>\n", argv[0]);
        exit(1);
    }

    threshold = atoi(argv[1]);
    nwindow = atoi(argv[2]);

    bzero(&this, sizeof(this));
    this.sin_family = AF_INET;
    this.sin_port = htons(PORT);
    this.sin_addr.s_addr = INADDR_ANY;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket error");
        exit(3);
    }

    if (bind(sock, (SA *)&this, sizeof(this)) < 0)
    {
        printf("Channel bind error\n");
        exit(4);
    }

    listen(sock, 10);
    len = sizeof(sendr);
    bzero(&sendr, len);

    if ((comm = accept(sock, (SA *)&sendr, &len)) < 0)
    {
        printf("Could not establish channel\n");
        exit(5);
    }

    printf("Channel established with %s at port %d\n",
           inet_ntoa(sendr.sin_addr), ntohs(sendr.sin_port));

    rlast = rnext = 0;

    while (1)
    {
        int n = recv(comm, &f, sizeof(f), 0);
        seqno = f.header.seq;

        if ((seqno != rnext) || (seqno < rlast - nwindow - 1))
        {
            printf("Received out of order frame %d\n", seqno);
            printf("Expecting %d frame\n", rnext);
            sendNak(comm, rnext, threshold);
        }
        else
        {
            rnext++;
            printf("New ordered frame = %d received...\n", f.header.seq);
            sendAck(comm, rnext, threshold);
            rlast = rnext;
        }

        if (n == 0)
            break; // connection closed
    }

    close(comm);
    close(sock);
    return 0;
}

Files to Save(two terminals)
• Receiver File : selective - repeat - recvr.c 
• Sender File : selective - repeat - sendr.c

                                      Compilation gcc selective
                   -
                   repeat - recvr.c - o receiver gcc selective - repeat - sendr.c - o sender

                                                                                        How to Run

                                                                                            ./
                                                                                        receiver 7 4 ./ sender 127.0.0.1 4 7(receiver IP)
