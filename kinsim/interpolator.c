#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "interpolator.h"

struct max_joint_steps{
	double step[joints];
};

struct min_axis_steps{
	double step[axis];
};

struct tool_offset{
	double offset[axis];
};

struct vec split(struct vec A, struct vec B, int steps){
	struct vec C;

	for(int i = 0; i < axis; i++){
		C.axis_pos[i] = A.axis_pos[i] + (B.axis_pos[i] - A.axis_pos[i]) / steps;
	}

	return(C);
}
/*struct vec tfkin(struct vec A, struct tool_offset t){
	struct vec a;
	a.axis_pos[0] = A.axis_pos[0] + t.offset[0];
}*/

//struct vec fikin(struct vec A, struct tool_offset t){}

struct vec fkin(struct vec A){ // forward kin axis -> joints
	struct vec a = A;
	double tmp;
	double jump;


	// wire kin:
	// A     B
	//  \   /
	//   \ /
	//    C
	//
	// A = (0/0)
	// B = (10/0)
	// AC = joint[0]
	// BC = joint[0]

	//a.joint_pos[0] = sqrt(pow(A.axis_pos[0], 2) + pow(A.axis_pos[1], 2)); // AC = sqrt(x^2+y^2)
	//a.joint_pos[1] = sqrt(pow(10 - A.axis_pos[0], 2) + pow(A.axis_pos[1], 2)); // BC = sqrt((10-x)^2+y^2)


	// floppy kin
	// rot disk = joint[0]
	// lin head = joint[1] (radius)
	tmp = atan2(A.axis_pos[1], A.axis_pos[0]);

	a.joint_pos[0] = tmp;
	a.joint_pos[1] = sqrt(pow(A.axis_pos[0], 2) + pow(A.axis_pos[1], 2));

	/*for(int i = 0; i < joints; i++){
		a.joint_pos[i] = A.axis_pos[i];
	}*/
	/*a.joint_pos[0] = A.axis_pos[0];
	a.joint_pos[1] = A.axis_pos[1];
	a.joint_pos[2] = A.axis_pos[2];
	a.joint_pos[3] = A.axis_pos[3];
	a.joint_pos[4] = A.axis_pos[4];
	a.joint_pos[5] = A.axis_pos[5];*/


	// cobra kin
	double dest_xy = sqrt(A.axis_pos[0] * A.axis_pos[0] + A.axis_pos[1] * A.axis_pos[1]);
	double dest_xyz = sqrt(dest_xy * dest_xy + (A.axis_pos[2] - 245) * (A.axis_pos[2] - 245));
	double tjoint1 = atan2(A.axis_pos[2] - 245, dest_xy);
	double tjoint2 = asin(dest_xyz / 2 / 190) * 2;

	a.joint_pos[0] = atan2(A.axis_pos[1], A.axis_pos[0]);
	a.joint_pos[1] = tjoint1 + 3.141526/2 - tjoint2/2 - 3.141526/4;
	a.joint_pos[2] = tjoint2 + 3.141526;
    
    a.joint_pos[2] = a.joint_pos[2] + a.joint_pos[1];

    
	return(a);
}

struct vec ikin(struct vec a){ // inverse kin joints -> axis
	struct vec A = a;

	// wire kin:
	// A     B
	//  \   /
	//   \ /
	//    C
	//
	// A = (0/0)
	// B = (10/0)
	// AC = joint[0]
	// BC = joint[0]

	A.axis_pos[0] = (pow(a.joint_pos[0], 2) - pow(a.joint_pos[1], 2) + pow(10, 2)) / (2 * 10);
	A.axis_pos[1] = sqrt(pow(a.joint_pos[0], 2) - pow(A.axis_pos[0], 2));


	// floppy kin
	// rot disk = joint[0]
	// lin head = joint[1] (radius)

	A.axis_pos[0] = sqrt(pow(a.joint_pos[1], 2) / ( 1+ pow(tan(a.joint_pos[0]), 2)));
	A.axis_pos[1] = tan(a.joint_pos[0]) * A.axis_pos[0];

	/*for(int i = 0; i < axis; i++){
		A.axis_pos[i] = a.joint_pos[i];
	}*/

	/*A.axis_pos[0] = a.joint_pos[0];
	A.axis_pos[1] = a.joint_pos[1];
	A.axis_pos[2] = a.joint_pos[2];
	A.axis_pos[3] = a.joint_pos[3];
	A.axis_pos[4] = a.joint_pos[4];
	A.axis_pos[5] = a.joint_pos[5];*/

	return(a);
}

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
    if(!A){
        A = (struct path *)malloc(sizeof(struct path));
        A->next = 0;
        A->prev = 0;
        A->pos = B;
        return;
    }
    while(A->next){
        A = A->next;
    }
	A->next = (struct path *)malloc(sizeof(struct path));
	A->next->pos = B;
    A->next->next = 0;
	A->next->prev = A;
}

int check_joint_steps(struct vec A, struct vec B, struct max_joint_steps max_j_s){
	for(int i = 0; i < joints; i++){
		if(fabs(A.joint_pos[i] - B.joint_pos[i]) > max_j_s.step[i]){
			return(1);
		}
	}
	return(0);
}

int check_axis_steps(struct vec A, struct vec B, struct min_axis_steps min_a_s){
	for(int i = 0; i < axis; i++){
		if(fabs(A.axis_pos[i] - B.axis_pos[i]) > min_a_s.step[i]){
			return(1);
		}
	}
	return(0);
}

int intp(struct path* A, struct max_joint_steps max_j_s, struct min_axis_steps min_a_s){
	struct path* p = A;
	int steps = 0;
	int tmp = 0;

	if(!A){
		return(0);
	}

	p->pos = fkin(p->pos);
	while(p->next){
		p->next->pos = fkin(p->next->pos);
		p = p->next;
	}

	p = A;

	while(p->next){
		for(int i = 0; i < axis; i++){
			tmp = ceil(abs((p->next->pos.axis_pos[i] - p->pos.axis_pos[i]) / min_a_s.step[i]));
			if(steps < tmp){
				steps = tmp;
			}
		}
		for(int i = 0; i < steps; i++){
			insert(p, split(p->pos, p->next->pos, steps - i)); // split
			p->next->pos = fkin(p->next->pos); // axis -> joints
			p = p->next;
		}

		steps = 0;

		p = p->next;
	}

	return(0);
}

int no_jump(struct path* AB){
	double tmp;
	while(AB && AB->next){
		tmp = round((AB->next->pos.joint_pos[0] - AB->pos.joint_pos[0]) / M_PI);
		AB->next->pos.joint_pos[0] -= tmp * M_PI;
		AB = AB->next;
	}
	return(0);
}

struct outpath interpol(struct path* AB){
    struct outpath p;
	struct path* head = AB;

	struct max_joint_steps max_j_s; // min axis res
	max_j_s.step[0] = 1 / 180 * 3.141526; // 0.1 deg res -> rad
	max_j_s.step[1] = 1 / 180 * 3.141526; // 0.1 mm res
	max_j_s.step[2] = 1 / 180 * 3.141526; // 0.1 mm res

	struct min_axis_steps min_a_s; // min joint res
	min_a_s.step[0] = 1; // 0.1 mm res
	min_a_s.step[1] = 1; // 0.1 mm res
	min_a_s.step[2] = 1; // 0.1 mm res

    
    intp(AB, max_j_s, min_a_s);
    
	int i = 0;
	head = AB;
	while(AB){
		i++;
		AB = AB->next;
	}
    p.length = i;
	AB = head;

	double *joint_pos_0 = (double *) malloc(sizeof(double) * i);
	double *joint_pos_1 = (double *) malloc(sizeof(double) * i);
	double *joint_pos_2 = (double *) malloc(sizeof(double) * i);
    
    p.jointpos1 = joint_pos_0;
    p.jointpos2 = joint_pos_1;
    p.jointpos3 = joint_pos_2;
    
	i = 0;
	while(AB){
		joint_pos_0[i] = AB->pos.joint_pos[0]/3.1415*180;
		joint_pos_1[i] = AB->pos.joint_pos[1]/3.1415*180;
		joint_pos_2[i] = AB->pos.joint_pos[2]/3.1415*180;

		i++;
		AB = AB->next;
	}

	return(p);
}

