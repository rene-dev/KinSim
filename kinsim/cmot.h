#include <math.h>
#include <iostream>

#define AXIS 5
#define JOINTS 5

struct point{
	double axis_pos[AXIS];
	double joint_pos[JOINTS];
	enum{
		axis,
		joint,
		both,
		invalid
	}type;
};

struct move{
	point dest;

	enum move_type{
		line,
		arc, // no full circle
		probe, // line, no blending, stop on probe signal
		fast, // fast move, no axis interpolation, joint jump allowed
		joint, // use joint position
    start // move to startpoint
	} type;

	double blend_r; // blending radius at end of line

	point dir; // start tangent of arc

	// limits by input
	double max_vel; // max axis drive velocity

	// limits by machine & look ahead
	double max_turn_vel; // limit by turn at end of move
	double max_break_vel; // limit by break distance to end of planed path

	// velocity calculated by vplan
	double start_vel; // joint velocity at start of move
	double end_vel; // joint velocity at end of move

	// move & submove ids
  unsigned int id; // move id
  unsigned int bid; // blend move id
  unsigned int iid; // move intp split id
	unsigned int tid; // move tplan split id

	bool trigger; // trigger new calculation
};

struct path{
	move m;
	path *next;
	path *prev;
};

enum kin_result{
	valid,
	valid_ambiguous,
	reachable_with_jump,
	not_reachable,
	joint_limit,
	colision,
	misc,
	fkin_not_implemented,
	ikin_not_implemented
};

struct tool_config{
	point offset; // axis offset of toolpoint
};

struct machine_config{
	double max_axis_vel[AXIS]; // velocity + accelaration limits
	double max_axis_acc[AXIS];
	double max_joint_vel[JOINTS];
	double max_joint_acc[JOINTS];

	double min_axis_pos[AXIS]; // pos limits
	double max_axis_pos[AXIS];
	double min_joint_pos[JOINTS];
	double max_joint_pos[JOINTS];

	point offset; // axis offset to zero

	double probe_break_dist; // max probe break distance

	enum{
		linear,
		radial
	} axis_type[AXIS],
    joint_type[JOINTS];

	//double joint_scale[JOINTS]; // joint rad, mm -> joint pos command
};

struct cmot_config{
	unsigned int trigger_dist; // #moves planing ahead
	unsigned int look_ahead; // velocity planing max look ahead

	double max_axis_step[AXIS]; // interpolation accuracy

	double timestep; // timestep for tplan

  bool blend; // use path blending
};

class machine{
	public:
		tool_config tool;
		machine_config cnf;
		virtual kin_result fkin(point from, point &to);
};

class cmot{
	private:
		path *work_path;

		path *end_of_blend;
		path *end_of_intp;
 		path *end_of_kin;
 		path *end_of_vplan;
 		path *end_of_tplan;
 		path *end_of_pop;

		bool fill_buffer; // request tplan & clean_up
    
        point pos; // current pos

	public:
		machine m;
		cmot_config cnf;

		void set_pos(point current_pos); // set current position

		void import(path *p); // import complex path, no copy

        void push(path p); // push path to buffer, no copy

        void push(move m); // push move to buffer, copy

		void init(); // initial fill of output buffer

		void blend(int steps); // path blending (line, line -> line, arc, line), -1 = blend all

		void intp(int steps); // interpolator, -1 = intp all

		kin_result kin(int steps); // kinematic (axis pos <-> joint pos), -1 = kin all
		kin_result kin(point current, point &dest); // kinematic (axis pos <-> joint pos)

		void vplan(int steps); // velocity planing, -1 = vplan all

		void tplan(int steps); // move / timestep planing, -1 = tplan all

        point pop(); // pop joint position from output buffer, trigger new calculation & clean_up

		void clean_up(); // remove old submoves from intp & tplan
};

void append(path *A, move B);
void insert(path *A, move B);
path *split(point A, move B, unsigned int count);
double length(move A);
