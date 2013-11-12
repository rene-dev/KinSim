//
//  interpolator.h
//  kinsim
//
//  Created by Rene on 12/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

#ifndef kinsim_interpolator_h
#define kinsim_interpolator_h

#define axis 3
#define joints 3

struct outpath{
    double* jointpos1;
    double* jointpos2;
    double* jointpos3;
    int length;
};

struct vec{
	double axis_pos[axis];
	double joint_pos[joints];
};

struct path{
	struct vec pos;
	struct path* next;
	struct path* prev;
};

struct outpath interpol(struct path* AB);
void append(struct path* A, struct vec B);

#endif
