#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int main(int argc, char *argv[]) {
        int opt;
        char *server;
        server = argv[argc - 1];
        // values of all inputed flags and values from command line
        int count = 0;
        int port_num = 0;
        int size = 0;
        float interval = 0.0;
	char *servPort;
        // boolean values for all expressed flags, turn to 1 if they are present
        int c = 0, i = 0, p = 0, s = 0, print = 0, Server = 0;
        while ((opt = getopt(argc, argv, "c:i:p:s:n:S")) != -1){
                switch (opt){
                        case 'c':
                                count = atoi(optarg);
                                c = 1;
                                break;
                        case 'i':
                                interval = atof(optarg);
                                i = 1;
                                break;
                        case 'p':
                                port_num = atoi(optarg);
				servPort = optarg;
                                p = 1;
                                break;
                        case 's':
                                size = atoi(optarg);
                                s = 1;
                                break;
                        case 'n':
                                print = 1;
                                break;
                        case 'S':
                                Server = 1;
                                break;
                        default: /* '?' */
                                fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        }
        //Print all statements made from the input with correct values
        if (c == 1){
                fprintf(stderr, "Count\t\t%15d\n", count);
        }
        if (s == 1){
                fprintf(stderr, "Size\t\t%15d\n", size);
        }
        if (i == 1){
                fprintf(stderr, "Interval\t%15.3f\n", interval);
        }
        if (p == 1){
                fprintf(stderr, "Port\t\t%15d\n", port_num);
        }
        if (print == 1){
                //Print IP Address
                fprintf(stderr, "Server_ip\t%15s\n", server);
                for(int i = 0; i < 10; i++){
                        fprintf(stderr, "*");
                        if(i == 9)
                                fprintf(stderr, "\n");
                }
        }
        if (print != 1)
                fprintf(stderr, "Server_ip\t%15s\n", server);
	
	
	//Tell the system what kind(s) of address info we want
	struct addrinfo addrCriteria;			//Criteria for address match
	memset(&addrCriteria, 0, sizeof(addrCriteria));	//Zero out structure
	addrCriteria.ai_family = AF_UNSPEC;		//Any address family

	//For the following fields, a zero value means "Don't Care"
	addrCriteria.ai_socktype = SOCK_DGRAM;		//Only datagram sockets
	addrCriteria.ai_protocol = IPPROTO_UDP;		// Only UDP protocol

	//Get address(es)
	struct addrinfo *servAddr;

	int rtnVal = getaddrinfo(server, servPort, &addrCriteria, &servAddr);

	if(rtnVal != 0)
		DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

	//Create a datagram/UDP socket
	int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
			servAddr->ai_protocol);	//Socket descriptor for client
	if(sock < 0)
		DieWithSystemMessage("socket() failed");

	char *str ="ping";

	//Send 'Size' to the server
	ssize_t numBytes = sendto(sock, str, size, 0, servAddr->ai_addr, servAddr->ai_addrlen);
	if(numBytes < 0)
		DieWithSystemMessage("socket() failed");
	else if(numBytes != size)
		DieWithUserMessage("sendto() error", "sent unexpected number of bytes");


	//Receive a response
	
	struct sockaddr_storage fromAddr;	//Source address of server
	//Set length of from address structure (in-out parameter)
	socklen_t fromAddrLen = sizeof(fromAddr);
	char buffer[MAXSTRINGLENGTH + 1]; // I/O buffer
	numBytes = recvfrom(sock, buffer, MAXSTRINGLENGTH, 0, (struct sockaddr *) &fromAddr, &fromAddrLen);
	if(numBytes < 0)
		DieWithSystemMessage("recvfrom() failed");
	else if(numBytes != size)
		DieWithUserMessage("recvfrom() error", "received unexpected number of bytes");
	//Verify reception from expected source
	if(!SockAddrsEqual(servAddr->ai_addr, (struct sockaddr *) &fromAddr))
		DieWithUserMessage("recvfrom()", "received a packet form unknown source");

	freeaddrinfo(servAddr);

	buffer[size] = '\0';	//Null-terminate received data
	int counter = 1;
	fprintf(stdout, "%d\t%15d\t%15.3f\n", counter, size, interval);	//Print the output

	close(sock);
	exit(0);
}
