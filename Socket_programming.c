#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h> #include <netinet/in.h> #include <arpa/inet.h> #include <unistd.h>
#include <stdlib.h>
#define BUFFERSIZE 1024 // arbitrary
// #define CHECK
int main(int argc, char *argv[])
{
    int sd, n;
    ssize_t i, j;
    char buffer[BUFFERSIZE];
    struct sockaddr_in server_addr; // An IPv4 address

    if (argc != 3)
    {
        printf("Usage: %s server-address server-port \n", argv[0]);
        printf("Example: %s 192.168.5.148 12345 \n", argv[0]);
        exit(1);
    }

    printf("TCP-echo-client starting...\n");
    // A TCP (IPv4 stream) socket is created.
    sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
    {
        perror("socket-call");
        exit(1);
    }
    /*
    The descriptor [sd] is not useful now. This is to be connected to server's address with [connect()] system call. Before making this call, the members of structure [ server_addr] are to be initialized.
    An IP socket address is defined as a combination of an IP interface address and a port number, using a protocol.
    IPv4 address is a structure [ struct sockaddr_in]
    The structure has the following members:
    struct sockaddr_in {
    sa_family_t
    u_int16_t
    sin_family;
    sin_port;
    struct in_addr sin_addr;
    // address family: AF_INET // port in network byte order // Internet address
    };


Internet address:
struct in addr{
u_int32_t
}:
s addr:
// address in network byte order
You may read manual [ ip(7) ].
The variable [server_addr], used by you, is an instance of the structure [ struct sockaddr_in]

*/
    // First member of structure [ server_addr] is initialized with the address family consistent with the
    // protocol family of the socket.
    server_addr.sin_family = AF_INET;

    /*
    The port number is to be converted from host byte order to network byte order.Network byte order is big endian, whereas host byte order may be little endian or big endian.In your PC(180x86) host byte order is little endian.The function[htons()] is used to convert port number from host byte order to network byte order.Even if the host byte order is big endian, calling[htons()] is not a mistake.You may read manual[htons(3)].argv[2] is the port number in our program.
    */

    // Second member of structure [server_addr] is initialised
    server_addr.sin_port = htons(atoi(argv[2]));

    /*
    Internet host address given as standard numbers -
        and - dots notation is to be converted into binary data and is to be stored as the[server_addr.sin_addr] of[server_addr] structure.The function[inet_aton()] is used to do that.This function returns nonzero if the address is valid,
        zero if not.You may read manual[inet_aton(3)].

    */
    // Third member of structure [server_addr] is initialised.
    // argv[1] is the server address in dotted decimal quad
    n = inet_aton(argv[1], &(server_addr.sin_addr));
    if (n == 0)
    {
        printf("inet_aton-Invalid address\n");
        exit(1);
    }
    n = connect(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n = -1)
    {
        perror("connect-call");
        exit(1);
    }
// the descriptor [sd] is now connected to server's socket
#ifdef CHECK
    printf("Check with [ # netstat -tn] in another terminal\n");
    printf("Check TCP connection establishment\n");
    while (1)
    {
        sleep(1);
    } // endless loop

#endif
    /*
        [bind()] system call was not called on[sd] before[connect()] system call.As[connect()] was called on a unbound socket, [sd] was automatically bound to a random free port(ephemeral port) with the local address set to[INADDR_ANY].
        */

    write(STDOUT_FILENO, "Enter the string:", 17);
    // clear buffer before reading memset(buffer, '0', BUFFERSIZE);
    i = read(STDIN_FILENO, buffer, BUFFERSIZE);
    if (i == -1)
    {
        perror("read1");
        exit(1);
    }
    printf("bytes read from keyboard-%u\n", i);
    // write contents of buffer on server's socket j-write( sd, buffer, i );
    if (j == -1)
    {
        perror("writel");
        exit(1);
    }
    printf("bytes written in server's socket-%u\n", j);
    // clear buffer before reading
    memset(buffer, '0', BUFFERSIZE); // read from server's socket into buffer
    i = read(sd, buffer, BUFFERSIZE);
    if (i - 1)
    {
        perror("read2");
        exit(1);
    }
    printf("bytes read from server's socket-%u\n", i);
    write(STDOUT_FILENO, "Reply from echo server->", 24);
    j = write(STDOUT_FILENO, buffer, i);
    if (j = -1)
    {
        perror("write2");
        exit(1);
    }
    // Shutdown the both-way (duplex) connection. shutdown(sd, SHUT_RDWR);
    return 0;
}

// Assignment - 1 : (a)Define CHECK in the program[fl.c].Compile and execute......
// $ gcc - Wall./ fl.c - o./ one - a
Run the program using wrong number of arguments to see usage message
// $ / one - a
// $ / one - a 192.168.5.251

// file-name f2.c TCP echo client //usage-> program-name server-address
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include << netinet / in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFERSIZE 1024

#define PORT
    7 int
    main(int argc, char *argv[])
{
    int sd, n;
    ssize_t i, j;
    char buffer[BUFFERSIZE];
    struct sockaddr_in server_addr;
    if (arge = !-2)
    {
        printf("Usage: %s server-address \n", argv[0]);
        exit(1);
    }
    sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd = -1)
    {
        perror("socket-call");
        exit(1);
    }
    server_addr.sin_family - AF_INET;
    server_addr.sin_port = htons(PORT);
    ninet_aton(argv[1], &(server_addr.sin_addr));
    if (n == 0)
    {
        printf("inet_aton-Invalid address\n");
        exit(1);
    }
    n - connect(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n - 1)
    {
        perror("connect-call");
        exit(1);
    }
    write(STDOUT_FILENO, "Enter the string:", 17);
    memset(buffer, '0', BUFFERSIZE);
    i = read(STDIN_FILENO, buffer, BUFFERSIZE);
    if (i - 1)
    {
        perror("read1");
        exit(1);
    }
    printf("bytes read from keyboard=%u\n", i);
    j - write(sd, buffer, i) : if (j = -1)
    {
        perror("write1");
        exit(1);
    }
    printf("bytes written in server's socket-%u\n", j);
    memset(buffer, "\0", BUFFERSIZE);
    i = read(sd, buffer, BUFFERSIZE);
    if (i == -1)
    {
        perror("read2");
        exit(1);
    }
    printf("bytes read from server's socket-%u\n", i);
    write(STDOUT_FILENO, "Reply from echo server->", 24);
    j = write(STDOUT_FILENO, buffer, i);
    if (j == -1)
    {
        perror("write2");
        exit(1);
    }

    shatdown(sd, SHUT_RDWR);

    return 0;
}

// file-name f3.c UDP echo client
//  usage> program-name server-address server-port
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define SIZE 512
// Uncomment the following line to use [recv from()] call in full form. //#define RECVFROM_FULL
// Uncomment the following line to use [recv from()] call in stripped form
// #define RECVFROM_STRIPPED
// Uncomment the following line to use [recv()] call
// #define RECV
// Uncomment the following line to use [read()] call
#define READ
// Uncomment the following line to print numeric values of some symbolic constants //#define DEBUG
int main(int argc, char *argv[])
{
    int sd; // socket descriptor
    int n;  // to check retumed value of function calls.

    ssize_t char
        buffer[SIZE];
    struct sockaddr_in server_addr;
    struct sockaddr_in source_addr;
#ifdef RECVFROM_FULL
    // following variables are used to illustrate use of [recvfrom ] call in full form
    socklen t unsigned long
        source addr length;
    unsigned long char
        nbo; // host address in network byte order hbo; // host address in network byte order *host_addr;
#endif
    if (arge != 3)
    {
        printf("usage prog-name server-address server-port \n");
        printf("example -> %s 172.16.2.15 7 \n", argv[0]);
        exit(1);
    }
    printf("udp-echo-client: PID of the program=%u\n", getpid());
    // an IPv4 datagram socket (UDP socket) is created
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // zero may be used for IPPROTO_UDP
    if (sd == -1)
    {
        perror("socket-call");
        exit(1);
    }
    // Initialise the members of server address server_addr.sin_family - AF_INET;
    n = inet_aton(argv[1], &(server addr.sin_addr));
    if (n == 0)
    {
        perror("invalid-address");
        exit(1);
    }
    server_addr.sin_port = htons(atoi(argv[2]));
    write(STDOUT_FILENO, "u-e-c: Enter the string:", 24);
    memset(buffer, 0, SIZE);
    i = read(STDIN_FILENO, buffer, SIZE);
    n = sendto(sd, buffer, i, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n == -1)
    {
        perror("sendto-call");
        exit(1);
    }
    printf("u-e-c: Sent %u bytes to server \n", n);
    // clear the buffer, before receiving from server
    memset(buffer, "\0", SIZE);

#ifdef RECVFROM FULL
    printf("u-e-c: Using recvfrom() call in full form \n");
    source_addr_length = sizeof(source_addr);
    n - recvfrom(sd, buffer, SIZE, MSG_PEEK, (struct sockaddr *)&source_addr, &source_addr_length);
    if (n = -1)
    {
        perror("recvfrom-call-full");
        exit(1);
    }
    // Print the value of a symbolic constant we are interested in
    printf("recvfrom-full: AF_INET-%u \n", AF_INET);
    /* [recvfrom()] system call has filled in members of the variable [source_addr]. The values of the members are now retrieved. */
    printf("rf-full: source address family = %u\n", source_addr.sin_family);
    nbo source_addr.sin_addr.s_addr;
    printf("rf-full: source address in network byte order ");
    printf("%LX Hex \n", nbo);
    hbontohl(source_addr.sin_addr.s_addr);
    printf("rf-full: source address in host byte order = %IX Hex \n", hbo);
    host_addr = inet_ntoa(source_addr.sin_addr);
    printf("rf-full: source address in dotted decimal quad=%s\n", host_addr);
    printf("rf-full: source port in network byte order = %X Hex \n", source_addr.sin_port);
    printf("rf-full: source port in host byte order - %X Hex\n", ntohs(source_addr.sin_port));
    printf("rf-full: source address length - %u bytes\n", source_addr_length);
#endif
#ifdef RECVFROM_STRIPPED
    printf("u-e-c: Using recvfrom() call in stripped form \n");
    // We are not interested in source address
    n - recvfrom(sd, buffer, SIZE, 0, NULL, 0);
    if (n == -1)
    {
        perror("recvfrom-call-stripped");
        exit(1);
    }
#endif
#ifdef RECV
    printf("u-e-c: Using recv() call \n");
    // We are not interested in source address but want to use [flag] n = recv(sd, buffer, SIZE, MSG_PEEK);
    if (n - 1)
    {
        perror("recv from-call-stripped");
        exit(1);
    }
#endif

#ifdef READ
    printf("u-e-c: Using read() call \n");
    // We are not interested in source address and don't want to use [flag] n-read( sd, buffer, SIZE);
    if (n - 1)
    {
        perror("read-call");
        exit(1);
    }
#endif
    printf("u-e-c: Received %u bytes from server \n", n);
    write(STDOUT_FILENO, "u-e-c: From server->", 20);
    write(STDOUT_FILENO, buffer, n);
    shutdown(sd, SHUT_RDWR);
    return 0;
}