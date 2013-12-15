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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	double axis_pos[AXIS];
	double joint_pos[JOINTS];
    enum {
		axis,
		joint,
		both,
		invalid
	} type;
} vec;

typedef struct path_t {
	vec pos;
	struct path_t* next;
	struct path_t* prev;
} path;
    
typedef struct {
    double* jointpos1;
    double* jointpos2;
    double* jointpos3;
    int length;
} outpath;

void append(path** A, vec B);
void insert(path* A, vec B);
void freepath(path* p);

#ifdef __cplusplus
}
#endif

#endif
