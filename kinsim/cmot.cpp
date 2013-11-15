#include "cmot.h"


void cmot::import(complex_path *p){ // import complex path
	cpath = p;
}

void cmot::start(){ // start calculation
}

void cmot::blend(){ // path blending (line, line -> line, arc, line)
}

void cmot::pplan(){ // path planing (arc -> line, ...)
	complex_path *p = cpath;

	while(p){
		if(p->move.type == complex_move::arc | p->move.type == complex_move::probe){
			p->move.type = complex_move::line;
		}
		p = p->next;
	}
}

void cmot::intp(){ // interpolator
	complex_path *p = current_intp;
    complex_path *tmp_path = 0;
	int split_count = 0;
	int tmp = 0;
    double dist = 0;
    double time = 0;
    unsigned int aid = 0;
    unsigned int jid = 0;

	if(!p){
		return;
	}

	while(p->next){
		if(p->move.type == complex_move::line){
			for(int i = 0; i < AXIS; i++){
				tmp = ceil(fabs((p->next->move.dest.axis_pos[i] - p->move.dest.axis_pos[i]) / cnf.max_axis_step[i]));
				if(tmp > split_count){
					split_count = tmp;
				}
			}
		}
        
        for(int i = 0; i < AXIS; i++){ // TODO: should not mix joint types for velocity
            dist += pow((p->next->move.dest.axis_pos[i] - p->move.dest.axis_pos[i]), 2);
        }
        
        time = sqrt(dist) / p->move.max_vel;
        
        aid = 0;
        tmp_path = split(p->move, p->next->move, split_count);
		for(complex_path *i = tmp_path; i->next; i = i->next){
            dist = 0;
            for(int j = 0; j < JOINTS; j++){
                dist += pow((i->next->move.dest.axis_pos[j] - i->move.dest.axis_pos[j]), 2);
            }
            
            i->next->move.max_joint_vel = sqrt(dist) / time;
            i->move.aid = aid;
            aid++;
		}
	}
}

void cmot::kin(){ // kinematic (axis pos -> joint pos)
}

void cmot::vplan(){ // velocity planing
}

void cmot::tplan(){ // move / timestep planing
}

void cmot::jdrive(){ // joint driver
}

joint_step_path *cmot::exportp(){ // export joint path
	return(jpath);
}