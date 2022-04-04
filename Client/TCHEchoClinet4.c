#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[])
{
  int opt;
  char *ip_addr;
  ip_addr = argv[argc - 1];
  // values of all inputed flags and values from command line
  int count = 0;
  int port_num = 0;
  int size = 0;
  float interval = 0.0;
  // boolean values for all expressed flags, turn to 1 if they are present
  int c = 0, i = 0, p = 0, s = 0, print = 0, Server = 0;
  while ((opt = getopt(argc, argv, "c:i:p:s:nS")) != -1)
  {
    switch (opt)
    {
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
      fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }


  //Print all statements made from the input with correct values
  if (c == 1)
  {
    fprintf(stderr, "Count               %d\n", count);
  }
  if (s == 1)
  {
    fprintf(stderr, "Size                %d\n", size);
  }
  if (i == 1)
  {
    fprintf(stderr, "Interval     %f\n", interval);
  }
  if (p == 1)
  {
    fprintf(stderr, "Port               %d\n", port_num);
  }

  //Print IP Address
  fprintf(stderr, "Server_ip        %s\n", ip_addr);

  char *servIP = ip_addr;     // First arg: server IP address (dotted quad)
  //char *echoString = argv[2]; // Second arg: string to echo
  char *echoString = "HELLO WORLD";

  // Third arg (optional): server port (numeric).  7 is well-known echo port
  in_port_t servPort = (argc == port_num); //? atoi(argv[3]) : 7;

  // Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family
  // Convert address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(servPort); // Server port

  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() failed");

  size_t echoStringLen = strlen(echoString); // Determine input length

  // Send the string to the server
  ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
  if (numBytes < 0)
    DieWithSystemMessage("send() failed");
  else if (numBytes != echoStringLen)
    DieWithUserMessage("send()", "sent unexpected number of bytes");

  // Receive the same string back from the server
  unsigned int totalBytesRcvd = 0; // Count of total bytes received
  fputs("Received: ", stdout);     // Setup to print the echoed string
  while (totalBytesRcvd < echoStringLen)
  {
    char buffer[BUFSIZE]; // I/O buffer
    /* Receive up to the buffer size (minus 1 to leave space for
     a null terminator) bytes from the sender */
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    if (numBytes < 0)
      DieWithSystemMessage("recv() failed");
    else if (numBytes == 0)
      DieWithUserMessage("recv()", "connection closed prematurely");
    totalBytesRcvd += numBytes; // Keep tally of total bytes
    buffer[numBytes] = '\0';    // Terminate the string!
    fputs(buffer, stdout);      // Print the echo buffer
  }

  fputc('\n', stdout); // Print a final linefeed

  close(sock);
  exit(0);
}