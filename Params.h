#ifndef PARAMS_H
#define PARAMS_H

// ** These parameters should not be modified after being passed to the Simulation or Model **

// Simulation progression parameters
struct ProgressionParams {
	int num_runs = 1; // number of simulation replicates to run
	int max_t = 1000; // maximum simulated time (in days)
};

// Model area parameters
struct AreaParams {
	int num_pat = 50; // number of population sites chosen for the simulation
	double side = 1.0; // size of the square simulation area (side x side) (km)
};

// Model life-process parameters
struct LifeParams {
	double mu_j = 0.05; // juvenile density independent mortality rate per day
	double mu_a = 0.125; // adult mortality rate per day
	double beta = 100.0; // parameter that controls mating rate
	double theta = 9.0; // average egg laying rate of wildtype females (eggs per day)
	double alpha0 = 100000.0; // baseline contribution to carrying capacity
	double mean_dev = 15.0; // mean juvenile development time (in days)
	int min_dev = 10; // minimum development time for a juvenile (in days)
};

// Gene drive inheritance parameters
struct InheritanceParams {
	double gamma = 0.025; // rate of r2 allele formation from W/D meiosis
	double xi = 0.2; // somatic Cas9 expression fitness cost
	double e = 0.95; // homing rate in females
};

// Gene drive release model parameters
struct ReleaseParams {
	int driver_start = 200; // time to start releasing drive alleles into the mosquito population
	int num_driver_M = 1000; // number of drive heterozygous (WD) male mosquitoes per release
	int num_driver_sites = 5; // number of gene drive release sites per year
};

// Dispersal model parameters
struct DispersalParams {
	double disp_rate = 0.01; // adult dispersal rate
	double max_disp = 0.2; // maximum distance at which two sites are connected (km)
};

// Aestivation model parameters
struct AestivationParams {
	double psi = 0.0; // aestivation rate
	double mu_aes = 0.0; // aestivation mortality
	int t_hide1 = 0; // start day of aestivation-entering period (day number of the year), not included
	int t_hide2 = 0; // end day of aestivation-entering period (day number of the year)
	int t_wake1 = 0; // start day of aestivation-waking period (day number of the year), not included
	int t_wake2 = 0; // end day of aestivation-waking period (day number of the year)
};

// Initial population values for the model
struct InitialPopsParams {
	int initial_WJ = 10000; // array of number of initial juvenile mosquitoes with wild homozygous (WW) genotype for each age group
	int initial_WM = 50000; // number of initial adult male mosquitoes with wild homozygous (WW) genotype
	int initial_WV = 10000; // number of initial adult unmated female (virgin) mosquitoes with wild homozygous (WW) genotype
	int initial_WF = 40000; // number of initial adult mated female mosquitoes with wild homozygous (WW) genotype
};

// Data-recording parameters
struct RecordParams {
	// recording window and intervals
	int rec_start = 0; // start time for the data recording window (in days) (non-inclusive)
	int rec_end = 1000; // end time for the data recording window (in days) (inclusive)
	int rec_interval_global = 1; // time interval for global data recording/output
	int rec_interval_local = 200; // time interval at which to collect/record local data (in days)
	int rec_sites_freq = 1; // fraction of sites to collect local data for (1 is all sites, 10 is 1 in 10 etc)

	// output filename labels
	int set_label = 1; // 'set of runs' index label for output files
};

#endif //PARAMS_H