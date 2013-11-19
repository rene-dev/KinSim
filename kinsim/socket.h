//
//  socket.h
//  kinsim
//
//  Created by crinq on 18.11.13.
//  Copyright (c) 2013 Rene. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#ifndef kinsim_socket_h
#define kinsim_socket_h


#define PORT 7000
#define BSIZE 128

int swrite(char request[]);
int sopen(char h[]);
void sclose();
char *sread();


#endif
