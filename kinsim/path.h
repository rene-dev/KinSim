//
//  path.h
//  kinsim
//
//  Created by Rene on 14/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#ifndef kinsim_path_h
#define kinsim_path_h

#define axis 3
#define joints 3

struct vec{
	double axis_pos[axis];
	double joint_pos[joints];
};

struct path{
	struct vec pos;
	struct path* next;
	struct path* prev;
};

void append(struct path* A, struct vec B);
void insert(struct path* A, struct vec B);
#endif
