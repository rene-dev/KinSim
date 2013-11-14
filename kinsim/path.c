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

void insert(struct path* A, struct vec B){
	struct path* tmp = A->next;
	A->next = (struct path *)malloc(sizeof(struct path));
	A->next->pos = B;
	A->next->next = tmp;
	A->next->prev = A;
	if(A->next->next){
		A->next->next->prev = A->next;
	}
}

void append(struct path* A, struct vec B){
    while(A->next){
        A = A->next;
    }
	A->next = (struct path *)malloc(sizeof(struct path));
	A->next->pos = B;
    A->next->next = 0;
	A->next->prev = A;
}

void freepath(struct path* p){
    struct path* tmp = p;
    while(tmp){
        if(tmp)
            free(tmp);
            tmp = tmp->next;
            }
}