#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <array>
#include "constants.h"
#include "Params.h"
#include "Patch.h"
#include "Dispersal.h"
#include "Aestivation.h"
#include "GDRelease.h"

using namespace constants;

// Runs the model.
class Model {
public:
	Model(AreaParams *area, InitialPopsParams *initial, LifeParams *life, AestivationParams *aes, DispersalParams *disp_param, 
		ReleaseParams *rel);
	~Model();
	void initiate();
	void run(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &inher_fraction);

	long long int calculate_tot_J(); 
	long long int calculate_tot_M();
	long long int calculate_tot_V();
	long long int calculate_tot_F();
	std::array<long long int, num_gen> calculate_tot_M_gen();
	std::vector<Patch*> get_sites() const;

private:
	std::vector<Patch*> sites;
	Dispersal* dispersal;
	Aestivation* aestivation;
	GDRelease* gd_release;

	// simulation area parameters
	int num_pat; // number of population sites chosen for the simulation
	double side; // size of the square simulation area (side x side) (km)

	// initial population values - common for all Patches
	InitialPopsParams *initial_pops;

	// juvenile development parameters - common for all Patches
	int min_dev; // minimum development time for a juvenile (in days)
	std::array<double, max_dev+1> dev_duration_probs; // array of probabilities of juvenile development duration for a new juvenile
	// (index indicates the number of days to develop or, equivalently, the age class the new juvenile starts at)

	// initiation methods
	void populate_sites();
	void set_dev_duration_probs(int min_time, int max_time);

	// life-processes - interface with Patch
	void run_step(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &inher_fraction);
	void juv_get_older();
	void adults_die();
	void virgins_mate();
	void lay_eggs(const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &inher_fraction);
	void juv_eclose();
};

#endif //MODEL_H