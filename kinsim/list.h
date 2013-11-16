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

template <class T>
struct list{
	T data;
	struct path* next;
	struct path* prev;
};

template <class T>
void append(struct list<T>** A, T B);

#endif /* defined(__kinsim__list__) */
