#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include <vector>
#include <string>
#include "constants.h"
#include "Params.h"
#include "InputParams.h"
#include "Point.h"

using namespace constants;

// Sets up and controls the flow of the simulation.
class Simulation {
public:
	Simulation(InputParams input);
	~Simulation();
	void set_coords(const std::string& filename);
	void set_boundary_type(BoundaryType boundary);
	void set_dispersal_type(DispersalType disp);
	void set_rainfall(const std::string& filename);
	void set_inheritance(InheritanceParams inher_params); 
	void run_reps();

private:
	int num_runs; // number of simulation replicates to run
	int max_t; // maximum simulated time (in days)

	ModelParams *model_params; // model parameters
	RecordParams *rec_params; // data-recording parameters

	SineRainfallParams *sine_rainfall_params;
	InputRainfallParams *input_rainfall_params;
	double alpha0_mean; // seasonality parameter
	double alpha0_variance; // seasonality parameter

	// additional parameter options
	std::vector<Point> sites_coords; // 2D coordinates for the sites on the simulated square
	BoundaryType boundary_type;
	DispersalType disp_type;

	// inheritance
	// f_ijk is the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
	std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> inher_fraction;
};

#endif //SIMULATION_H