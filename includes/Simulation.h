#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include <vector>
#include <string>
#include "constants.h"
#include "Params.h"
#include "Point.h"

using namespace constants;

// Sets up and controls the flow of the simulation.
class Simulation {
public:
	Simulation(ProgressionParams &prog, AreaParams &area, LifeParams &life, ReleaseParams &rel, DispersalParams &disp,
	 AestivationParams &aes, InitialPopsParams &initial, RecordParams &rec, double a0_mean, double a0_var, double a1, double ampl);
	void set_coords(const std::string& filename);
	void set_boundary_type(BoundaryType boundary);
	void set_dispersal_type(DispersalType disp);
	void set_rainfall(double resp, const std::string& filename);
	void set_inheritance(InheritanceParams inher_params); 
	void run_reps();

private:
	int num_runs; // number of simulation replicates to run
	int max_t; // maximum simulated time (in days)

	AreaParams *area_params; // model area parameters
	LifeParams *life_params; // model life-process parameters
	ReleaseParams *rel_params; // gene drive release model parameters
	DispersalParams *disp_params; // dispersal model parameters
	AestivationParams *aes_params; // aestivation model parameters
	InitialPopsParams *initial_params; // initial population values
	RecordParams *rec_params; // data-recording parameters
	double alpha0_mean; // seasonality parameter
	double alpha0_variance; // seasonality parameter
	double alpha1; // seasonality parameter
	double amp; // seasonality parameter
	double resp; // seasonality parameter

	// additional parameter options
	std::vector<Point> sites_coords; // 2D coordinates for the sites on the simulated square
	BoundaryType boundary_type;
	DispersalType disp_type;
	std::vector<double> rainfall; // daily rainfall for every day (whether in a year cycle, or max_t days)

	// inheritance
	// f_ijk is the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
	std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> inher_fraction;
};

#endif //SIMULATION_H