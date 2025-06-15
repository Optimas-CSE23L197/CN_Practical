// file-name fl.c
// four different sockets are created by defining A or B or C or D
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include << netinet / in.h>
#include <arpa/inet.h>
// #define A
// #define B
// #define C
// #define D
int main(void)
{
    int int
        sd : // to be used as socket-descriptor
             n;
    struct sockaddr_in d;
    printf("pid=%u\n", getpid());
#ifdef A
    printf("Assignment-la.. stream socket in local namespace\n");
    sd = socket(PF_UNIX, SOCK_STREAM, 0);
    // PF_UNIX may be replaced with PF_LOCAL or PF_FILE if( sd == -1) { perror("socket-call"); exit(1); }
    printf("descriptor=%u\n", sd);
#endif
#ifdef B
    printf("Assignment-lb... datagram socket in local namespace \n");

    sd = socket(PF_UNIX, SOCK_DGRAM, 0);
    if (sd == -1)
    {
        perror("socket-call");
        exit(1);
    }
    printf("descriptor=%u\n", sd);
#endif
#ifdef C
    printf("Assignment-le... Internet IPv4 stream socket(TCP)\n");
    sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
    {
        perror("socket-call");
        exit(1);
    }
    printf("descriptor=%u\n", sd);
    *A newly created TCP socket has no remote or local address and is not fully specified.To identify the socket created in this experiment, we listen on this socket, though the socket is not yet bound to any address.This is done only to identify the type of the socket.* / n = listen(sd, 1);
    if (n = -1)
    {
        perror("listen");
        exit(1);
    }
/* When listen() is called on a unbound socket, the socket is automatically bound to a random free port with the local address set to all local interfaces */
#endif
#ifdef D
    printf("Assignment-1d... Internet IPv4 datagram socket(UDP)\n");
    sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd = -1)
    {
        perror("socket-call");
        exit(4);
    }
    printf("descriptor=%u\n", sd);
    /* When a UDP socket is created, its local and remote addresses are unspecified. [ listen ] call was used to identify the socket created in Assignment-le. This call is allowed on a stream socket. As UDP is not a stream socket, method used in Assignment-le is not used in this assignment.
    To identify the created socket, the socket is bound to an Internet address 127.0.0.1 (loopback interface). [ bind system call is used to do that. An Internet address is a structure with three members. At present ignore the calls [inet_aton()], [htons()] and [bind()] calls. */
    // address family
    d.sin_family = AF_INET;
    // port
}
d.sin_port = htons(0); // any port
// Internet address
if (inet_aton("127.0.0.1", &(d.sin_addr)) == 0)
{
}
printf("invalid-addr\n");
exit(1);
// the created socket is bound to address [d]
n = bind(sd, (struct sockaddr *)&d, sizeof(d));
if (n = -1)
{
    perror("bind");
    exit(1);
}
#endif
while (1)
{
    sleep(1);
} // endless loop return 0;
// The above program enters an endless loop after creating socket.This enables us to check creation of
// the sockets.

The following program is a simple server using connection oriented protocol(stream socket), in local namespace(Unix domain socket).Save the following file as[f2.c]
// file-name f2.c
//  ECHO server using stream UNIX domain socket.
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#define PATH "/tmp/aa"
#define BUFFERSIZE 2048
                                                                                            int main(void)
{
    int xxx; // to be used as socket descriptor new_sock_fd; // socket descriptor
    int n;
    ssize t i, j;
    char buff[BUFFERSIZE];
    socklen_t addr_len;
    struct sockaddr_un server_address;
    // remove stale socket, if any from a previous run
    n = unlink(PATH);
    if (n == -1)
        perror("unlink");
    printf("pid=%u\n", getpid());
    // UNIX domain socket is created.
    // PF_UNIX or PF_LOCAL or PF_FILE may be used as protocol family
    // xxx = socket(PF_UNIX, SOCK_STREAM, 0);

    // xxx= socket(PF_LOCAL, SOCK_STREAM, 0); //xxx = socket(PF_FILE, SOCK_STREAM, 0);
    XXX = socket(PF LOCAL, SOCK STREAM, 0);
    if (xxx = -1)
    {
        perror("socket-call");
        exit(1);
    }
    printf("socket descriptor=%u\n", xxx);
    // uncomment next three lines for Assignment-4
    // umask(0000);
    // n- fchmod( xxx,0707);
    // if(n-1) { perror("chmod"); }
    /*
    The data type for Unix domain socket is [ struct sockaddr_un ]
    This has the following two members:
    struct sockaddr_un
    {
        short int char
            sun_family   // AF_UNIX
                sun path // pathname of address
                */
    // the above two members are initialised
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, PATH, sizeof(server_address.sun_path));
    /* size of the address is determined with macro [ SUN_LEN ], Address length is needed as one of the arguments of bind ] system call. */
    addr_len = SUN_LEN(&server_address);
    /* An address is to be assigned to the [socket] as the local endpoint of communication. This is done using the [bind] system call. Read [bind(2)]. Note that the Unix domain socket address is cast to a generic data type [ struct sockaddr ]. */
    n = bind(xxx, (struct sockaddr *)&server_address, addr_len);
    if (n - 1)
    {
        perror("bind-call");
        exit(1);
    }
    /* Then [listen system call is used, to be in readiness to accept incoming connections to the socket */

    n = listen(xxx, 5);

    if (n = -1)
    {
        perror("listen-call");
        exit(1);
    }

    /*The second parameter[5] indicates that five incoming requests can be kept in queue.Read manual[listen(2)] */

    /*Then[accept] system call is used to accept incoming request.This system call blocks,
    till a client's request comes on the socket. [accept] system call extracts the first connection request on the queue of pending connections, creates a new connected socket and allocates a new descriptor for the socket. In the following lines this descriptor is named [ new_sock_fd ], which the [accept] system call returns. This new descriptor, is full duplex (it can be read from and can be written into) and communication is done with the client with it. Read manual [accept(2)]. */

    while (1) // In this endless loop a single client request is served

    {

        printf("Waiting for a client's connection\n");

        new_sock_fd = accept(xxx,

        );

        (struct sockaddr *)&amp;
        server_address,

            &amp;
        addr len

            if (new sock_fd - 1)
        {
            perror("accept");
            exit(1);
        }

        memset(buf, '0', BUFFERSIZE);

        i = read(new_sock_fd, buf, BUFFERSIZE);

        if (i == -1)
        {
            perror("read-call");
            close(new_sock_fd);
        }
        printf("uds-server: read %u byte request from client\n", i);

        j = write(new_sock_fd, buf, i);

        if (j == -1)
        {
            perror("write");
            exit(1);
        }

        printf("uds-server: written %u byte reply to client\n", j);
        close(new_sock_fd);

    } // while ends

    return 0;

} // end of main

#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#define BUFFERSIZE 1024
#define SERVER "/tmp/aa"
int main(void)
{
    int
        sock_fd;
    int
        n;
    socklen t
        length;
    ssize_t
        ij : char
                 buffer[BUFFERSIZE];
    struct sockaddr_un unix_addr;
    /* uncomment any one of the following three lines */ // sock_fd=socket( PF_UNIX, SOCK_STREAM, 0); //sock_fd= socket(PF_LOCAL, SOCK_STREAM, 0); sock_fd=socket(PF_FILE, SOCK_STREAM, 0); if(sock_fd=-1){ perror("socket"); exit(1); }
    unix_addr.sun_family = AF_UNIX;
    strncpy(unix_addr.sun_path, SERVER, sizeof(unix_addr.sun_path));
    length = SUN_LEN(&unix_addr);
    n connect(sock_fd, (struct sockaddr *)&unix_addr, length) : if (n = -1)
    {
        perror("connect");
        exit(1);
    }
    // clear the buffer with null characters
    memset(buffer, '\0', BUFFERSIZE);
    write(STDOUT_FILENO, "Enter a string ", 15);
    i = read(STDIN_FILENO, buffer, BUFFERSIZE);
    // writing to server socket
    j = write(sock_fd, buffer, i);
    if (j =
            -1)
    {
        perror("write");
        exit(1);
    }
    // clear the buffer with null characters
    memset(buffer, '\0', BUFFERSIZE); // reading from server socket
    i = read(sock_fd, buffer, BUFFERSIZE);
    if (i == -1)
    {
        perror("read");
        exit(1);
    }
    write(STDOUT_FILENO, "Received from server->", 22);
    j = write(STDOUT_FILENO, buffer, i);
    if (j == -1)
    {
        perror("write-2");
        exit(1);
    }
    return 0;
}