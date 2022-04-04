#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int main(int argc, char *argv[]) {
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
        while ((opt = getopt(argc, argv, "c:i:p:s:nS")) != -1){
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
        //Print IP Address
        fprintf(stderr, "Server_ip\t%15s\n", ip_addr);

}
