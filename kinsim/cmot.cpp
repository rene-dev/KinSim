#include "cmot.h"

using namespace std;

double lenght(point A, struct move B){
    double dist = 0;
    switch(B.type){
        case move::arc:
            cout << "cmot: length: arc move not jet supported, treated as line move, move id: " << B.id << " bid: " << B.bid << endl << flush;
        case move::joint:
        case move::probe:
        case move::start:
        case move::fast:
        case move::line:
            for(int i = 0; i < AXIS; i++){
                dist += pow((A.axis_pos[i] - B.dest.axis_pos[i]), 2);
            }
            dist = sqrt(dist);
        break;
    }
    return(dist);
}

list<struct move> *split(point A, struct move B, unsigned int count){
    list<struct move> *result = 0;

    switch(B.type){
        case move::arc:
            cout << "cmot: split: arc move not jet supported, treated as line move, move id: " << B.id << " bid: " << B.bid << endl << flush;
        case move::joint:
        case move::probe:
        case move::start:
        case move::fast:
        case move::line:
            //TODO
            break;
    }

    return(result);
}

void cmot::set_pos(point p){
    pos = p;
}

void cmot::push(struct move m){
    
}

void cmot::push(list<struct move> *p){

}

void cmot::import(list<struct move> *p){ // import complex path
	work_path = p;
}

void cmot::init(){
}

void cmot::blend(int steps){ // path blending (line, line -> line, arc, line)
    list<struct move> *p = end_of_blend;

    if(!p){
		return;
	}

    while(p->next && p->next->next){
        if(p->next->data.blend_r != 0){
            // TODO: check length
            // calc arc
            // insert arc
            // set id, bid
        }
        p = p->next;
    }
}

void cmot::intp(int steps){ // interpolator
	list<struct move> *p = end_of_intp;
    list<struct move> *tmp_path = 0;
	int split_count = 0;
	int tmp = 0;
    double dist = 0;
    double time = 0;
    unsigned int iid = 0;

	if(!p){
		return;
	}

	while(p->next){
		if(p->data.type == move::line){
			for(int i = 0; i < AXIS; i++){
				tmp = ceil(fabs((p->next->data.dest.axis_pos[i] - p->data.dest.axis_pos[i]) / cnf.max_axis_step[i]));
				if(tmp > split_count){
					split_count = tmp;
				}
			}
		}

        for(int i = 0; i < AXIS; i++){ // TODO: should not mix joint types for velocity
            dist += pow((p->next->data.dest.axis_pos[i] - p->data.dest.axis_pos[i]), 2);
        }

        time = sqrt(dist) / p->data.max_vel;

        iid = 0;
        tmp_path = split(p->data.dest, p->next->data, split_count);
		for(list<struct move> *i = tmp_path; i->next; i = i->next){
            dist = 0;
            for(int j = 0; j < JOINTS; j++){
                dist += pow((i->next->data.dest.axis_pos[j] - i->data.dest.axis_pos[j]), 2);
            }

            i->next->data.max_vel = sqrt(dist) / time;
            i->data.iid = iid;
            iid++;
		}
	}
}

kin_result cmot::kin(point current, point &dest){ // kinematic (axis pos <-> joint pos)
    return(m.kin(current, dest));
}

kin_result cmot::kin(int steps){
    return(valid);
}

kin_result machine::kin(point from, point &to){
    switch(to.type){
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
    return(valid);
}

void cmot::vplan(int steps){ // velocity planing

}

void cmot::tplan(int steps){ // move / timestep planing
}

void cmot::clean_up(){ // remove old submoves from intp & tplan
}

point cmot::pop(){
    return(end_of_pop->data.dest);
}
