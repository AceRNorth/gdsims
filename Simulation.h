#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include "constants.h"
#include "Params.h"

using namespace constants;

// Sets up and controls the flow of the simulation.
class Simulation {
public:
	Simulation(ProgressionParams &prog, AreaParams &area, LifeParams &life, ReleaseParams &rel, DispersalParams &disp,
	 AestivationParams &aes, InitialPopsParams &initial, RecordParams &rec); 
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

	// inheritance
	// f_ijk is the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
	std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> inher_fraction;
};

#endif //SIMULATION_H