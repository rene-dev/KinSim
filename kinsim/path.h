//
//  path.h
//  kinsim
//
//  Created by Rene on 14/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//



#ifndef kinsim_path_h
#define kinsim_path_h

#define AXIS 3
#define JOINTS 3

struct vec{
	double axis_pos[AXIS];
	double joint_pos[JOINTS];
    enum{
		axis,
		joint,
		both,
		invalid
	}type;
};

struct path{
	struct vec pos;
	struct path* next;
	struct path* prev;
};

void append(struct path** A, struct vec B);
void insert(struct path* A, struct vec B);
void freepath(struct path* p);
#endif
