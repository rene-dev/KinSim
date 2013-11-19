//
//  socket.c
//  kinsim
//
//  Created by crinq on 18.11.13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#include "socket.h"

int so;


int sopen(char h[]){
    struct hostent *host;
    struct sockaddr_in addr;
    
    host = gethostbyname(h);
    if (!host)
    {
        herror("gethostbyname() failed");
        return 2;
    }
    addr.sin_addr = *(struct in_addr*)host->h_addr;
    
    so = socket(PF_INET, SOCK_STREAM, 0);
    if (so == -1)
    {
        perror("socket() failed");
        return 3;
    }
    
    addr.sin_port = htons(PORT);
    addr.sin_family = AF_INET;
    
    if (connect(so, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("connect() failed");
        return 4;
    }
    
    return(0);
}

void sclose(){
    close(so);
}

int swrite(char request[]){
    if (send(so, request, strlen(request), 0) == -1)
    {
        perror("send() failed");
        return 1;
    }
    
    return 0;
}

char *sread(){
    char response[BSIZE];
    int bytes;
    
    while((bytes = recv(so, response, sizeof(response), 0)) > 0){
        fwrite(response, 1, bytes, stdout);
    }
    
    return(response);
}
