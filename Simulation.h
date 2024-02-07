#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include <string> // for error messages
#include "constants.h"
#include "Params.h"

using namespace constants;

void out_of_bounds_msg(const std::string& par);
void invalid_interval_msg(const std::string& param1, const std::string& param2);

// Sets up and controls the flow of the simulation.
class Simulation {
public:
	Simulation(ProgressionParams &prog, AreaParams &area, LifeParams &life, InheritanceParams &inher,
 		ReleaseParams &rel, DispersalParams &disp, AestivationParams &aes, InitialPopsParams &initial, RecordParams &rec); 
	void set_inheritance(); 
	void run_reps();

private:
	int num_runs; // number of simulation replicates to run
	int max_t; // maximum simulated time (in days)

	AreaParams *area_params; // model area parameters
	LifeParams *life_params; // model life-process parameters
	InheritanceParams *inher_params; // gene drive inheritance parameters
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