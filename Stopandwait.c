


stop-wait-receiver.c
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>

#define SA struct sockaddr
#define MAXBUF 512
#define PORT 50000
#define STR "This is our data"

void sendAck(int comm, int ack, int threshold);

struct head {
    int seqno, ackno;
};

struct frame {
    struct head header;
    char data[MAXBUF];
};

int main(int argc, char **argv){
    struct sockaddr_in this, sendr;
    int sock, comm, len;
    int seq, ack = 0;
    struct frame f;

    if(argc != 2){
        printf("\tUsage: %s <noise threshold level>\n", argv[0]);
        exit(1);
    }

    bzero(&this, sizeof(this));
    this.sin_family = AF_INET;
    this.sin_port = htons(PORT);
    this.sin_addr.s_addr = INADDR_ANY;

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket error ");
        exit(3);
    }

    if(bind(sock, (SA *)&this, sizeof(this)) < 0){
        printf("Channel bind error\n");
        exit(4);
    }

    listen(sock, 10);
    len = sizeof(sendr);
    bzero(&sendr, len);

    if((comm = accept(sock, (SA *)&sendr, &len)) < 0){
        printf("could not establish channel\n");
        exit(5);
    }

    printf("Channel established with %s at port %d\n", 
           inet_ntoa(sendr.sin_addr), ntohs(sendr.sin_port));

    for(;;){
        int m = recv(comm, &f, sizeof(f), 0);
        seq = f.header.seqno;
        if(seq == ack){ 
            printf("Old frame received.\n");
            printf("Discarding frame ......\n");
            sendAck(comm, ack, atoi(argv[1]));
        }
        else { 
            printf("Frame %d received\n", seq);
            if(seq == 0) ack = 1;
            if(seq == 1) ack = 0;
            sendAck(comm, ack, atoi(argv[1]));
        }
        if(m == 0) break;
    }

    close(comm);
    close(sock);
}

void sendAck(int comm, int ack, int threshold){
    int noise, len;
    struct frame fr;

    noise = rand() % 10;

    if(noise < threshold){
        printf("Sending ACK = %d\n", ack);
        fr.header.seqno = -1;
        fr.header.ackno = ack;
        strcpy(fr.data, STR);
        send(comm, &fr, sizeof(fr), 0);
    }
    else {
        printf("Channel is too noisy! ACK is lost\n");
    }
return 0;
}



stop-wait-sender.c

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<signal.h>
#include<fcntl.h>

#define SA struct sockaddr
#define MAXBUF 512
#define PORT 50000
#define STR "This is our data"
#define TIMEOUT 2

struct itimerval value;
struct frame {
    struct head {
        int seqno, ackno;
    } header;
    char data[MAXBUF];
};

void handler(int sig){
    int n, ack, seq;
    struct frame f;
    switch(sig){
        case SIGIO:
            n = recv(sock, &f, sizeof(f), 0);
            ack = f.header.ackno;
            printf("ACK %d received\n", ack);

            fr.header.seqno = ack;
            fr.header.ackno = -1;
            strcpy(fr.data, STR);
            send(sock, &fr, sizeof(fr), 0);
            printf("Sending new frame %d\n", ack);

            value.it_value.tv_sec = TIMEOUT;
            setitimer(ITIMER_REAL, &value, 0);
            break;

        case SIGALRM:
            printf("Frame %d retransmitted.....\n", fr.header.seqno);
            send(sock, &fr, sizeof(fr), 0);
            value.it_value.tv_sec = TIMEOUT;
            setitimer(ITIMER_REAL, &value, 0);
            break;
    }
}

int sock;
struct frame fr;

int main(int argc, char **argv){
    struct sockaddr_in recvr;
    int flags;
    struct sigaction ioact, alrmact;

    if(argc != 2){
        printf("\tUsage: %s <IP address of the receiver>\n", argv[0]);
        exit(1);
    }

    bzero(&recvr, sizeof(recvr));
    recvr.sin_family = AF_INET;
    recvr.sin_port = htons(PORT);
    if(inet_aton(argv[1], &recvr.sin_addr) == 0){
        printf("Address conversion error\n");
        exit(2);
    }

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket error: ");
        exit(3);
    }

    if(connect(sock, (SA *)&recvr, sizeof(recvr)) < 0){
        printf("No channel could be established\n");
        exit(4);
    }

    printf("Channel established\n");

    // Setup SIGIO
    ioact.sa_handler = handler;
    sigemptyset(&ioact.sa_mask);
    sigaddset(&ioact.sa_mask, SIGALRM);
    sigaction(SIGIO, &ioact, NULL);

    // Setup SIGALRM
    alrmact.sa_handler = handler;
    sigemptyset(&alrmact.sa_mask);
    sigaction(SIGALRM, &alrmact, NULL);

    // Async I/O
    fcntl(sock, F_SETOWN, getpid());
    fcntl(sock, F_SETFL, O_ASYNC);

    // Send frame 0
    fr.header.seqno = 0;
    fr.header.ackno = -1;
    strcpy(fr.data, STR);
    printf("Frame 0 transmitted\n");
    send(sock, &fr, sizeof(fr), 0);

    value.it_value.tv_sec = TIMEOUT;
    setitimer(ITIMER_REAL, &value, 0);

    while(1){
        sleep(10);
    }

    close(sock);
}





Compile the Code

gcc stop-wait-receiver.c -o receiver
gcc stop-wait-sender.c -o sender

Run the Receiver First
./receiver 5   (The number 5 here is the threshold for "noise" — you can adjust it between 0–10.)

Then Run the Sender
./sender 127.0.0.1

Sample Output
Receiver:


Channel established with 127.0.0.1 at port 50000
Frame 0 received
Sending ACK = 1
Frame 1 received
Sending ACK = 0

Sender:

Channel established
Frame 0 transmitted
ACK 1 received
Sending new frame 1
ACK 0 received
Sending new frame 0
