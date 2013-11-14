t#include<math>

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
}

struct complex_move{
	enum move_type{
		line,
		arc, // no full circle
		probe, // line, no blending
		fast, // fast move, no interpolation
		joint // use joint position
	} type;

	double blend_r; // blending radius at end of line
	point dest; // destination of move
	point dir; // start tangent of arc
	double probe_break_dist; // max probe break distance
	double vel; // drive velocity
	double max_turn_vel[JOINTS]; // limit by turn
	double max_break_vel[JOINTS]; // limit by break distance
	double drive_vel[JOINTS]; // joint velocity
}

struct joint_step_path{ // output of
	point dest;
	joint_step_path *next;
	joint_step_path *prev;
}

struct complex_path{
	complex_move move;
	complex_path *next;
	complex_path *prev;
}

enum kin_result{
	valid,
	valid_ambiguous,
	reachable_with_jump,
	not_reachable,
	joint_limit,
	colision,
	misc
}

struct tool_config{
	point offset;
}

struct machine_config{
	double max_axis_vel[AXIS]; // velocity + accelaration limits
	double max_axis_acc[AXIS];
	double max_joint_vel[JOINTS];
	double max_joint_acc[JOINTS];

	double min_axis_pos[AXIS]; // pos limits
	double max_axis_pos[AXIS];
	double min_joint_pos[JOINTS];
	double max_joint_pos[JOINTS];

	double joint_scale[JOINTS]: // joint rad, mm -> joint pos command
}

struct cmot_config{
	unsigned int max_look_ahead; // look ahead limit, 0 = always stop at end of move, -1 = unlimited look ahead

	double max_axis_step[AXIS]; // interpolation accuracy
	double max_joint_step[JOINTS];

	double timestep; // timestep for tplan
}

class machine{
	public:
		tool_config tool;
		machine_config cnf;
		virtual kin_result fkin(point from, point &to);
}

class cmot{
	private:
		complex_path *cpath;
		joint_step_path *jpath;

	public:
		machine m;
		cmot_config cnf;

		void import(complex_path *p); // import complex path

		void start(); // start calculation

		void blend(); // path blending (line, line -> line, arc, line)

		void pplan(); // path planing (arc -> line, ...)

		void intp(); // interpolator

		void fkin(); // kinematic (axis pos -> joint pos)

		void vplan(); // velocity planing

		void tplan(); // move / timestep planing

		void jdrive(); // joint driver

		joint_step_path *export(); // export joint path
}
