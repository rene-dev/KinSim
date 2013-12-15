#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "interpolator.h"

#ifndef M_PI
#define M_PI 3.14159f
#endif

struct max_joint_steps{
	double step[JOINTS];
};

struct min_axis_steps{
	double step[AXIS];
};

struct tool_offset{
	double offset[AXIS];
};

vec split(vec A, vec B, int steps){
	vec C;

	for(int i = 0; i < AXIS; i++){
		C.axis_pos[i] = A.axis_pos[i] + (B.axis_pos[i] - A.axis_pos[i]) / steps;
	}

	return(C);
}
/*vec tfkin(vec A, struct tool_offset t){
	vec a;
	a.axis_pos[0] = A.axis_pos[0] + t.offset[0];
}*/

//vec fikin(vec A, struct tool_offset t){}

vec fkin(vec A){ // forward kin axis -> joints
	vec a = A;
	double tmp;


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

vec ikin(vec a){ // inverse kin joints -> axis
	vec A = a;

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

int check_joint_steps(vec A, vec B, struct max_joint_steps max_j_s){
	for(int i = 0; i < JOINTS; i++){
		if(fabs(A.joint_pos[i] - B.joint_pos[i]) > max_j_s.step[i]){
			return(1);
		}
	}
	return(0);
}

int check_axis_steps(vec A, vec B, struct min_axis_steps min_a_s){
	for(int i = 0; i < AXIS; i++){
		if(fabs(A.axis_pos[i] - B.axis_pos[i]) > min_a_s.step[i]){
			return(1);
		}
	}
	return(0);
}

int intp(path* A, struct max_joint_steps max_j_s, struct min_axis_steps min_a_s){
	path* p = A;
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
		for(int i = 0; i < AXIS; i++){
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

int no_jump(path* AB){
	double tmp;
	while(AB && AB->next){
		tmp = round((AB->next->pos.joint_pos[0] - AB->pos.joint_pos[0]) / M_PI);
		AB->next->pos.joint_pos[0] -= tmp * M_PI;
		AB = AB->next;
	}
	return(0);
}

void interpol(path* AB){
	path* head = AB;

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
	AB = head;

	i = 0;
	while(AB){
		AB->pos.joint_pos[0]/=3.1415*180;
		AB->pos.joint_pos[1]/=3.1415*180;
		AB->pos.joint_pos[2]/=3.1415*180;

		i++;
		AB = AB->next;
	}
}
