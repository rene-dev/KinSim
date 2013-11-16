//
//  list.cpp
//  kinsim
//
//  Created by crinq on 15.11.13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#include "list.h"





template <class T>
void append(struct list<T>** A, T B){
    struct list<T>* tmp = *A;
    if(!tmp){
        tmp = (list<T>*)malloc(sizeof(list<T>));
        tmp->next = 0;
        tmp->prev = 0;
        tmp->data = B;
        *A = tmp;
        return;
    }
    while(tmp->next){
        tmp = tmp->next;
    }
	tmp->next = (list<T>*)malloc(sizeof(list<T>));
	tmp->next->data = B;
    tmp->next->next = 0;
	tmp->next->prev = *A;
}