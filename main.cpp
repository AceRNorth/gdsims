#include <iostream>
#include <limits>
#include <chrono>
#include <vector>
#include "Simulation.h"
#include "Params.h"

int main()
{	
	ProgressionParams prog;
	AreaParams area;
	LifeParams life;
	InheritanceParams inher;
	ReleaseParams rel;
	DispersalParams disp;
	AestivationParams aes;
	InitialPopsParams initial;
	RecordParams rec;
	
	// input parameters
	// progression parameters
	int num_runs;
	int max_t; 

	// area parameters
	int num_pat;  
	double side;
	
	// model parameters
	double mu_j;
	double mu_a;
	double beta;
	double theta;
	double alpha0;
	double mean_dev;
	int min_dev;

	// gene drive inheritance parameters
	double gamma;
	double xi;
	double e;

	// gene drive release parameters
	int driver_start;
	int num_driver_M;
	int num_driver_sites; 

	// dispersal parameters 
	double disp_rate; 
	double max_disp; 

	// aestivation parameters
	double psi;
	double mu_aes;
	int t_hide1;
	int t_hide2;
	int t_wake1; 
	int t_wake2;

	// data-recording parameters
	int rec_start; 
	int rec_end;
	int rec_interval_global;
	int rec_interval_local;
	int rec_sites_freq; 
	int set_label; 

// ****** old bit
	bool invalid_input2 = true;
	std::cout << "Please enter the input parameters for the model:" << std::endl;
	while (invalid_input2) {
		std::cin.clear();

		std::cin >> num_runs; 
		std::cin >> max_t;
		std::cin >> num_pat; 
		std::cin >> side; 
		std::cin >> mu_j;
		std::cin >> mu_a; 
		std::cin >> beta; 
		std::cin >> theta; 
		std::cin >> alpha0; 
		std::cin >> mean_dev; 
		std::cin >> min_dev;
		std::cin >> gamma; 
		std::cin >> xi;
		std::cin >> e;
		std::cin >> driver_start;
		std::cin >> num_driver_M; 
		std::cin >> num_driver_sites;
		std::cin >> disp_rate;
		std::cin >> max_disp;
		std::cin >> psi;
		std::cin >> mu_aes;
		std::cin >> t_hide1;
		std::cin >> t_hide2;
		std::cin >> t_wake1; 
		std::cin >> t_wake2;
		std::cin >> rec_start;
		std::cin >> rec_end;
		std::cin >> rec_interval_global;
		std::cin >> rec_interval_local;
		std::cin >> rec_sites_freq;
		std::cin >> set_label;

		// input validation
		if (num_runs <= 0) {out_of_bounds_msg("num_runs"); continue;}
		if (max_t <= 0) {out_of_bounds_msg("max_t"); continue;}
		if (num_pat <= 0) {out_of_bounds_msg("num_pat"); continue;}
		if (side <= 0) {out_of_bounds_msg("side"); continue;}
		if (mu_j < 0 || mu_j >= 1) {out_of_bounds_msg("mu_j"); continue;}
		if (mu_a <= 0 || mu_a >= 1) {out_of_bounds_msg("mu_a"); continue;}
		if (beta <= 0) {out_of_bounds_msg("beta"); continue;}
		if (theta <= 0) {out_of_bounds_msg("theta"); continue;}
		if (alpha0 <= 0) {out_of_bounds_msg("alpha0"); continue;}
		if (mean_dev <= 0) {out_of_bounds_msg("mean_dev"); continue;}
		if (min_dev <= 0) {out_of_bounds_msg("min_dev"); continue;}
		if (gamma < 0 || gamma > 1) {out_of_bounds_msg("gamma"); continue;}
		if (xi < 0 || xi > 1) {out_of_bounds_msg("xi"); continue;}
		if (e < 0 || e > 1) {out_of_bounds_msg("e"); continue;}
		if (driver_start < 1) {out_of_bounds_msg("driver_start"); continue;}
		if (num_driver_M < 0) {out_of_bounds_msg("num_driver_M"); continue;}
		if (num_driver_sites < 0) {out_of_bounds_msg("num_driver_sites"); continue;}
		if (disp_rate < 0 || disp_rate > 1) {out_of_bounds_msg("disp_rate"); continue;}
		if (max_disp <= 0 || max_disp > side) {out_of_bounds_msg("max_disp"); continue;}
		if (psi < 0 || psi > 1) {out_of_bounds_msg("psi"); continue;}
		if (mu_aes < 0 || mu_aes > 1) {out_of_bounds_msg("mu_aes"); continue;}
		if (psi > 0) {
			if (t_hide1 < 1 || t_hide1 > 365) {out_of_bounds_msg("t_hide1"); continue;}
			if (t_hide2 < 1 || t_hide2 > 365) {out_of_bounds_msg("t_hide2"); continue;}
			if (t_wake1 < 1 || t_wake1 > 365) {out_of_bounds_msg("t_wake1"); continue;}
			if (t_wake2 < 1 || t_wake2 > 365) {out_of_bounds_msg("t_wake2"); continue;}
		}
		if (rec_start < 0) {out_of_bounds_msg("rec_start"); continue;}
		if (rec_end < 0) {out_of_bounds_msg("rec_end"); continue;}
		if (rec_interval_global < 1) {out_of_bounds_msg("rec_interval_global"); continue;}
		if (rec_interval_local < 1) {out_of_bounds_msg("rec_interval_local"); continue;}
		if (rec_sites_freq < 1) {out_of_bounds_msg("rec_sites_freq"); continue;}
		if (set_label < 0) {out_of_bounds_msg("set_label"); continue;}

		if (rec_start >= rec_end) {invalid_interval_msg("rec_start", "rec_end"); continue;}
		if (psi > 0) {
			if (t_hide1 >= t_hide2) {invalid_interval_msg("t_hide1", "t_hide2"); continue;}
			if (t_wake1 >= t_wake2) {invalid_interval_msg("t_wake1", "t_wake2"); continue;}
		}

		if (num_driver_sites > num_pat) {
			std::cout << "Warning: num_driver_sites > num_pat. ";
			std::cout << "This simulation will have reduced gene drive release sites num_driver_sites = num_pat." << std::endl;
		}
		if (driver_start > max_t || num_driver_sites == 0 || num_driver_M == 0) {
			std::cout << "Warning: num_driver_sites or num_driver_M = 0, or driver_start > max_t. ";
			std::cout << "This simulation will not include gene drive." << std::endl;
			std::cout << "Inheritance parameters xi, e and gamma will have no effect." << std::endl;
		} 
		if (disp_rate == 0 || max_disp == 0) {
			std::cout << "Warning: disp_rate or max_disp = 0. This simulation will not include dispersal." << std::endl;
		} 
		if (max_disp > side/2) {
			std::cout << "Warning: max_disp > side/2." << std::endl;
		}
		if (t_hide1 > max_t || t_hide2 > max_t || t_wake1 > max_t || t_wake2 > max_t) {
			std::cout << "Warning: the aestivation interval times are larger than max_t. ";
			std::cout << "This simulation will only run partly through the aestivation period." << std::endl;
		}
		if (psi == 0) {
			std::cout << "Warning: psi = 0. This simulation will not include aestivation." << std::endl;
		} 

		if (rec_start > max_t) {
			std::cout << "Warning: rec_start > max_t. This simulation will not include local recording." << std::endl;
		}
		if (max_t - rec_interval_local - rec_start < 0) {
			std::cout << "Warning: the interval between rec_start and max_t is larger than rec_interval_local. ";
			std::cout << "This simulation will only record local data for day 0." << std::endl;
		}

		invalid_input2 = false;
	}

	prog.num_runs = num_runs;
	prog.max_t =  max_t;
	area.num_pat = num_pat;
	area.side = side;
	life.mu_j = mu_j;
	life.mu_a = mu_a;
	life.beta = beta;
	life.theta = theta;
	life.alpha0 = alpha0;
	life.mean_dev = mean_dev;
	life.min_dev = min_dev;
	inher.gamma = gamma;
	inher.xi = xi;
	inher.e = e;
	rel.driver_start = driver_start;
	rel.num_driver_M = num_driver_M;
	rel.num_driver_sites = num_driver_sites;
	disp.disp_rate = disp_rate;
	disp.max_disp = max_disp;
	aes.psi = psi;
	aes.mu_aes = mu_aes;
	aes.t_hide1 = t_hide1;
	aes.t_hide2 = t_hide2;
	aes.t_wake1 = t_wake1;
	aes.t_wake2 = t_wake2;
	rec.rec_start = rec_start;
	rec.rec_end = rec_end;
	rec.rec_interval_global = rec_interval_global;
	rec.rec_interval_local = rec_interval_local;
	rec.rec_sites_freq = rec_sites_freq;
	rec.set_label = set_label;


	auto start = std::chrono::steady_clock::now();

	// run simulation
	Simulation simulation(prog, area, life, rel, disp, aes, initial, rec);
	simulation.set_inheritance(inher);
	simulation.run_reps();

	auto finish = std::chrono::steady_clock::now();
	double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
	std::cout << "Program run time: " << std::endl;
	printf("%.10f\n", elapsed_seconds);
	
// *** old bit
	
	return 0;
}
