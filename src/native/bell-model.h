#include <vector>

#ifndef M_PI
#define M_PI  (3.1415926535)
#endif

#ifndef BOOL
#	define BOOL		int
#	define TRUE		1
#	define FALSE	0
#endif

struct mat_point;

typedef struct mat_point
{
	double m;
	double x, y, z;
	double x0, y0, z0;
	double vx, vy, vz;

	std::vector<mat_point*> connected_v;
	bool makes_sound;
	bool fixed;

	// non-public values - used during the calculation process
	double ax, ay, az;

}
mat_point;

typedef struct model_data
{
	double elasticity, friction;
	std::vector<mat_point*> points_v;
}
model_data;

// Calculation parameters
typedef struct model_calc_params
{
	// Multiplier of the target sound.
	// Can be lowered during the process
	// of calculation to prevent overloads
	double gain;

	// Calculation steps per frame
	int steps;

	// Time delta between steps (step longitude)
	double dt;

	// Physics model data
    model_data* data;
}
calc_params;

double do_calc(model_calc_params* params);
