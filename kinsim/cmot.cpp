#include "cmot.h"

//using namespace std;

template <unsigned int AXIS, unsigned int JOINTS>
point<AXIS, JOINTS> point<AXIS, JOINTS>::operator+(point<AXIS, JOINTS> right){
    point result;
    for(int i = 0; i < AXIS; i++){
        result.axis_pos[i] = axis_pos[i] + right.axis_pos[i];
    }
    for(int i = 0; i < JOINTS; i++){
        result.joint_pos[i] = joint_pos[i] + right.joint_pos[i];
    }
    return(result);
}

template <unsigned int AXIS, unsigned int JOINTS>
point<AXIS, JOINTS> point<AXIS, JOINTS>::operator-(point<AXIS, JOINTS> right){
    point result;
    for(int i = 0; i < AXIS; i++){
        result.axis_pos[i] = axis_pos[i] - right.axis_pos[i];
    }
    for(int i = 0; i < JOINTS; i++){
        result.joint_pos[i] = joint_pos[i] - right.joint_pos[i];
    }
    return(result);
}

template <unsigned int AXIS, unsigned int JOINTS>
point<AXIS, JOINTS> operator*(double left, point<AXIS, JOINTS> right){
    point<AXIS, JOINTS> result;
    for(int i = 0; i < AXIS; i++){
        result.axis_pos[i] = left * right.axis_pos[i];
    }
    for(int i = 0; i < JOINTS; i++){
        result.joint_pos[i] = left * right.joint_pos[i];
    }
    return(result);
}

template <unsigned int AXIS, unsigned int JOINTS>
point<AXIS, JOINTS> point<AXIS, JOINTS>::operator*(double right){
    point result;
    for(int i = 0; i < AXIS; i++){
        result.axis_pos[i] = axis_pos[i] * right;
    }
    for(int i = 0; i < JOINTS; i++){
        result.joint_pos[i] = joint_pos[i] * right;
    }
    return(result);
}

template <unsigned int AXIS, unsigned int JOINTS>
point<AXIS, JOINTS> point<AXIS, JOINTS>::operator/(double right){
    point result;
    for(int i = 0; i < AXIS; i++){
        result.axis_pos[i] = axis_pos[i] / right;
    }
    for(int i = 0; i < JOINTS; i++){
        result.joint_pos[i] = joint_pos[i] / right;
    }
    return(result);
}

template <unsigned int AXIS, unsigned int JOINTS>
double point<AXIS, JOINTS>::operator!(){
    double result = 0;
    
    switch(type){
        case point::axis:
            for(int i = 0; i < AXIS; i++){
                result += axis_pos[i] * axis_pos[i];
            }
        break;
            
        case point::joint:
            for(int i = 0; i < JOINTS; i++){
                result += joint_pos[i] * joint_pos[i];
            }
        break;
            
        default:
            break;
    }
    return(sqrt(result));
}


template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::set_pos(point<AXIS, JOINTS> p){
    pos = p;
}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::push(move<AXIS, JOINTS> m){
    work_path = *work_path + m;
}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::push(list<move<AXIS, JOINTS>> *p){
    work_path = *work_path + p;
}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::import(list<move<AXIS, JOINTS>> *p){ // import complex path
	work_path = p;
}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::init(){
}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::blend(int steps){ // path blending (line, line -> line, arc, line)
    list<move<AXIS, JOINTS>> *p = end_of_blend;

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

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::intp(int steps){ // interpolator
	list<move<AXIS, JOINTS>> *p = end_of_intp;
    move<AXIS, JOINTS> m;
	unsigned int split_count = 0;
	int tmp = 0;
    double dist = 0;
    double time = 0;
    unsigned int iid = 0;

	if(!p || p->next){
		return;
	}

    switch(p->data.type){
        case move<AXIS, JOINTS>::arc:
        case move<AXIS, JOINTS>::probe:
        case move<AXIS, JOINTS>::line:
            for(int i = 0; i < AXIS; i++){ // calc #splits
				tmp = ceil(fabs((p->next->data.dest.axis_pos[i] - p->data.dest.axis_pos[i]) / cnf.max_axis_step[i]));
				if(tmp > split_count){
					split_count = tmp;
				}
			}
            
            m = p->data;
//            m.dest =
        break;
            
        case move<AXIS, JOINTS>::fast:
        case move<AXIS, JOINTS>::joint:
        case move<AXIS, JOINTS>::start:
        break;
    }
    
    
	while(p->next){
		if(p->data.type == move<AXIS, JOINTS>::line){
			
		}

        for(int i = 0; i < AXIS; i++){ // TODO: should not mix joint types for velocity
            dist += pow((p->next->data.dest.axis_pos[i] - p->data.dest.axis_pos[i]), 2);
        }

        time = sqrt(dist) / p->data.max_vel;

        iid = 0;
     //   tmp_path = split(p->data.dest, p->next->data, split_count);
	/*	for(list<struct move> *i = tmp_path; i->next; i = i->next){
            dist = 0;
            for(int j = 0; j < JOINTS; j++){
                dist += pow((i->next->data.dest.axis_pos[j] - i->data.dest.axis_pos[j]), 2);
            }

            i->next->data.max_vel = sqrt(dist) / time;
            i->data.iid = iid;
            iid++;
		}*/
	}
}

template <unsigned int AXIS, unsigned int JOINTS>
kin_result cmot<AXIS, JOINTS>::kin(point<AXIS, JOINTS> current, point<AXIS, JOINTS> &dest){ // kinematic (axis pos <-> joint pos)
    return(m.kin(current, dest));
}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::kin(int steps){
}

template <unsigned int AXIS, unsigned int JOINTS>
kin_result machine<AXIS, JOINTS>::kin(point<AXIS, JOINTS> from, point<AXIS, JOINTS> &to){
    switch(to.type){
        case point<AXIS, JOINTS>::axis:
            std::cout << "cmot: kin: forward kinematic not implemented" << std::endl << std::flush;
            // TODO: axis -> joint
            break;
            
        case point<AXIS, JOINTS>::joint:
            // TODO: joint -> axis
            std::cout << "cmot: kin: forward kinematic not implemented" << std::endl << std::flush;
            break;
            
        default:
            break;
    }
    return(valid);
}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::vplan(int steps){ // velocity planing

}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::tplan(int steps){ // move / timestep planing
}

template <unsigned int AXIS, unsigned int JOINTS>
void cmot<AXIS, JOINTS>::clean_up(){ // remove old submoves from intp & tplan
}

template <unsigned int AXIS, unsigned int JOINTS>
point<AXIS, JOINTS> cmot<AXIS, JOINTS>::pop(){
    return(end_of_pop->data.dest);
}
