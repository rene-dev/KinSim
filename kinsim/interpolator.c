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
	double soffset[axis];
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
	max_j_s.step[0] = 0.1 / 180 * 3.141526; // 0.1 deg res -> rad
	max_j_s.step[1] = 0.1 / 180 * 3.141526; // 0.1 mm res
	max_j_s.step[2] = 0.1 / 180 * 3.141526; // 0.1 mm res

	struct min_axis_steps min_a_s; // min joint res
	min_a_s.step[0] = 50; // 0.1 mm res
	min_a_s.step[1] = 50; // 0.1 mm res
	min_a_s.step[2] = 50; // 0.1 mm res

    /*
    struct vec A;
	A.axis_pos[0] = 100;
	A.axis_pos[1] = 0;
	A.axis_pos[2] = 0;

	struct vec B;
	B.axis_pos[0] = 200;
	B.axis_pos[1] = 0;
	B.axis_pos[2] = 0;

    struct vec C;
	C.axis_pos[0] = 200;
	C.axis_pos[1] = 100;
	C.axis_pos[2] = 0;

    struct vec D;
	D.axis_pos[0] = 100;
	D.axis_pos[1] = 100;
	D.axis_pos[2] = 0;
    
    struct vec E;
	E.axis_pos[0] = 100;
	E.axis_pos[1] = 0;
	E.axis_pos[2] = 100;
    
    struct vec F;
	F.axis_pos[0] = 200;
	F.axis_pos[1] = 0;
	F.axis_pos[2] = 100;
	
    struct path* AB = (struct path *)malloc(sizeof(struct path));
	struct path* AB_head;
	AB->next = 0;
	AB->prev = 0;
	AB->pos = A;
    append(AB, B);
	append(AB, C);
	append(AB, D);
	append(AB, A);
    append(AB, E);
    append(AB, F);
    */


	//insert(AB, A);
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

	
	//no_jump(AB);
	/*while(AB){
		//printf("Axis Pos (%f/%f)", AB->pos.axis_pos[0], AB->pos.axis_pos[1]);
		//printf(" -> ");
		//printf("joint Pos (%f/%f)", AB->pos.joint_pos[0], AB->pos.joint_pos[1]);
		//printf("\n");

		//fprintf(file, "%f %f %f %f\n", AB->pos.axis_pos[0], AB->pos.axis_pos[1], AB->pos.joint_pos[0], AB->pos.joint_pos[1]);
		// plot with: gnuplot -e 'set multiplot layout 6,1; plot "intp.txt" using 0:1 title "axis[0]";plot "intp.txt" using 0:2 title "axis[1]";plot "intp.txt" using 0:3 title "joint[0]";plot "intp.txt" using 0:4 title "joint[1]";plot "intp.txt" using 1:2 title "axis"; plot "intp.txt" using 3:4 title "joints"' -p
		fprintf(file, "%f %f %f %f %f %f\n", AB->pos.axis_pos[0], AB->pos.axis_pos[1], AB->pos.axis_pos[2], AB->pos.joint_pos[0], AB->pos.joint_pos[1], AB->pos.joint_pos[2]);
		AB = AB->next;
	}*/

	//fclose(file);
	return(p);
}

