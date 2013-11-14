#include "cmot.h"

using namespace std;

void cmot::inport(complex_path *p){ // import complex path
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
	complex_path *p = cpath;

	if(!p){
		return();
	}

	while(p->next){
		for(int i = 0; i < AXIS; i++){

		}
}

void cmot::fkin(){ // kinematic (axis pos -> joint pos)
}

void cmot::vplan(){ // velocity planing
}

void cmot::tplan(){ // move / timestep planing
}

void cmot::jdrive(){ // joint driver
}

joint_step_path *cmot::export(){ // export joint path
	return(jpath);
}