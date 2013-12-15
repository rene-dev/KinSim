//
//  path.c
//  kinsim
//
//  Created by Rene on 14/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "path.h"

void insert(path* A, vec B){
	path* tmp = A->next;
	A->next = (path *)malloc(sizeof(path));
	A->next->pos = B;
	A->next->next = tmp;
	A->next->prev = A;
	if(A->next->next){
		A->next->next->prev = A->next;
	}
}

void append(path** A, vec B){
    path* tmp = *A;
    if(!tmp){
        tmp = (path *)malloc(sizeof(path));
        tmp->next = 0;
        tmp->prev = 0;
        tmp->pos = B;
        *A = tmp;
        return;
    }
    while(tmp->next){
        tmp = tmp->next;
    }
	tmp->next = (path *)malloc(sizeof(path));
	tmp->next->pos = B;
    tmp->next->next = 0;
	tmp->next->prev = *A;
}

void freepath(path* p){
    path* tmp;
    while(p){
        if(p)
            tmp = p;
            p = p->next;
            free(tmp);
            }
}














