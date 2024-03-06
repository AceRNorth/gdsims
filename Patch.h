#ifndef PATCH_H
#define PATCH_H

#include <array>
#include "Patch.h"
#include "constants.h"
#include "Params.h"
#include "Point.h"

using namespace constants;

// Contains the information of a local mosquito population
class Patch {
public:
	Patch(LifeParams* par, double side);
	Patch(LifeParams* par, Point point);
	void populate(int initial_WJ, int initial_WM, int initial_WV, int initial_WF);

	Point get_coords() const;
	std::array<long long int, num_gen> get_M() const;
	std::array<std::array<long long int, num_gen>, num_gen> get_F() const;

	long long int calculate_tot_J();
	long long int calculate_tot_M();
	long long int calculate_tot_V();
	long long int calculate_tot_F();

	// life-processes for the local site
	void juv_get_older(int max_dev);
	void adults_die();
	void virgins_mate();
	void lay_eggs(const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f,
	 const std::array<double, max_dev+1> &dev_duration_probs);
	void juv_eclose();
	void update_comp();
	void update_mate();
	
	// interface to Dispersal
	void M_disperse_out(const std::array<long long int, num_gen> &m_out);
	void F_disperse_out(const std::array<std::array<long long int, num_gen>, num_gen> &f_out);
	void M_disperse_in(int gen, long long int m_in);
	void F_disperse_in(int f_gen, int m_gen, long long int f_disp);

	// interface to Aestivation
	void F_hide(const std::array<std::array<long long int, num_gen>, num_gen> &f_try);
	void F_wake(const std::array<std::array<long long int, num_gen>, num_gen> &f_wake);

	// interface to GDRelease
	void add_driver_M(int num_driver_M);

private:
	LifeParams* params; 

	Point coords; // (x, y) coordinates of the site
	// number of juvenile mosquitoes with each genotype and in each age group.
	// age ordered from oldest (0 days left to eclosion) to youngest (max_dev - 1 days left)
	std::array<std::array<long long int, max_dev+1>, num_gen> J; 
	std::array<long long int, num_gen> M; // number of male mosquitoes with each genotype
	std::array<long long int, num_gen> V; // number of unmated female (virgin) mosquitoes with each genotype
	// number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j	
	std::array<std::array<long long int, num_gen>, num_gen> F; 

	long double comp; // survival probability per juvenile per day (both density-dependent and independent factors)
	long double mate_rate; // probability of an unmated (virgin) female mating on a given day
};

#endif //PATCH_H