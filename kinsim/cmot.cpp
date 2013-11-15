#include "cmot.h"

using namespace std;

double lenght(point A, complex_move B){
    double dist = 0;
    switch(B.type){
        case complex_move::arc:
            cout << "cmot: length: arc move not jet supported, treated as line move, move id: " << B.id << " bid: " << B.bid << endl << flush;
        case complex_move::joint:
        case complex_move::probe:
        case complex_move::start:
        case complex_move::fast:
        case complex_move::line:
            for(int i = 0; i < AXIS; i++){
                dist += pow((A.axis_pos[i] - B.dest.axis_pos[i]), 2);
            }
            dist = sqrt(dist);
        break;
    }
    return(dist);
}

complex_path *split(point A, complex_move B, unsigned int count){
    complex_path *result = 0;
    complex_move tmp;
    
    switch(B.type){
        case complex_move::arc:
            cout << "cmot: split: arc move not jet supported, treated as line move, move id: " << B.id << " bid: " << B.bid << endl << flush;
        case complex_move::joint:
        case complex_move::probe:
        case complex_move::start:
        case complex_move::fast:
        case complex_move::line:
            //TODO
            break;
    }
    
    return(result);
}

void cmot::import(complex_path *p){ // import complex path
	in_path = p;
}

void cmot::start(){ // start calculation
}

void cmot::blend(){ // path blending (line, line -> line, arc, line)
    complex_path *p = in_path;
    
    if(!p){
		return;
	}
    
    while(p->next && p->next->next){
        if(p->next->move.blend_r != 0){
            // TODO: check length
            // calc arc
            // insert arc
            // set id, bid
        }
        p = p->next;
    }
}

void cmot::intp(){ // interpolator
	complex_path *p = intp_pos;
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
        tmp_path = split(p->move.dest, p->next->move, split_count);
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

void cmot::kin(point &A){ // kinematic (axis pos -> joint pos)
    switch(A.type){
        case point::axis:
            cout << "cmot: kin: forward kinematic not implemented" << endl << flush;
            // TODO: axis -> joint
        break;
            
        case point::joint:
            // TODO: joint -> axis
            cout << "cmot: kin: forward kinematic not implemented" << endl << flush;
        break;
        
        default:
        break;
    }
}

void cmot::vplan(){ // velocity planing
    
}

void cmot::tplan(){ // move / timestep planing
}

void cmot::jdrive(){ // joint driver
}

joint_step_path *cmot::exportp(){ // export joint path
	return(out_path);
}