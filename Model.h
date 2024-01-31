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
	void run(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f);

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

	// initial population values for Patch
	int initial_WJ = 10000; // array of number of initial juvenile mosquitoes with wild homozygous (WW) genotype for each age group
	int initial_WM = 50000; // number of initial adult male mosquitoes with wild homozygous (WW) genotype
	int initial_WV = 10000; // number of initial adult unmated female (virgin) mosquitoes with wild homozygous (WW) genotype
	int initial_WF = 40000; // number of initial adult mated female mosquitoes with wild homozygous (WW) genotype

	// life-process parameters for Patch
	double mu_j; // juvenile density independent mortality rate per day
	double mu_a; // adult mortality rate per day
	double beta; // parameter that controls mating rate
	double theta; // average egg laying rate of wildtype females (eggs per day)
	double alpha0; // baseline contribution to carrying capacity
	double mean_dev; // mean juvenile development time (in days)
	int min_dev; // minimum development time for a juvenile (in days)
	std::array<double, max_dev+1> dev_duration_probs; // array of probabilities of juvenile development duration for a new juvenile
	// (index indicates the number of days to develop or, equivalently, the age class the new juvenile starts at)

	// initiation methods
	void populate_sites();
	void set_dev_duration_probs(int min_time, int max_time);

	// life-processes - interface with Patch
	void run_step(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f);
	void juv_get_older();
	void adults_die();
	void virgins_mate();
	void lay_eggs(const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f);
	void juv_eclose();
	//void update_comp();
	//void update_mate();

};

#endif //MODEL_H