//
//  list.h
//  kinsim
//
//  Created by crinq on 15.11.13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#ifndef __kinsim__list__
#define __kinsim__list__

#include <iostream>
#include <cstdlib>

template <class T>
struct list{
	T data;
	struct list* next;
	struct list* prev;
    
    list<T>* operator+(const T &right){
        struct list<T>* tmp = this;
        while(tmp->next){
            tmp = tmp->next;
        }
        tmp->next = (list<T>*)malloc(sizeof(list<T>));
        tmp->next->data = right;
        tmp->next->next = 0;
        tmp->next->prev = this;
        return(this);
    }
    
    list<T>* operator+(list<T>* right){
        struct list<T>* tmp = this;

        while(tmp->next){
            tmp = tmp->next;
        }
        
        tmp->next = right;
        tmp->next->prev = tmp;
        
        return(this);
    }

};

template <class T>
void append(struct list<T>* A, T B);


template <class T>
void insert(const struct list<T>** A, const T B);

#endif /* defined(__kinsim__list__) */
