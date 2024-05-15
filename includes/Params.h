#ifndef PARAMS_H
#define PARAMS_H

// ** These parameters should not be modified after being passed to the Simulation or Model **

// Simulation progression parameters
struct ProgressionParams {
	int num_runs; // number of simulation replicates to run
	int max_t; // maximum simulated time (in days)
};

// Model area parameters
struct AreaParams {
	int num_pat; // number of population sites chosen for the simulation
	double side; // size of the square simulation area (side x side) (km)
};

// Model life-process parameters
struct LifeParams {
	double mu_j; // juvenile density independent mortality rate per day
	double mu_a; // adult mortality rate per day
	double beta; // parameter that controls mating rate
	double theta; // average egg laying rate of wildtype females (eggs per day)
	double mean_dev; // mean juvenile development time (in days)
	int min_dev; // minimum development time for a juvenile (in days)
};

// Gene drive inheritance parameters
struct InheritanceParams {
	double gamma; // rate of r2 allele formation from W/D meiosis
	double xi; // somatic Cas9 expression fitness cost
	double e; // homing rate in females
};

// Gene drive release model parameters
struct ReleaseParams {
	int driver_start; // time to start releasing drive alleles into the mosquito population
	int num_driver_M; // number of drive heterozygous (WD) male mosquitoes per release
	int num_driver_sites; // number of gene drive release sites per year
};

// Dispersal model parameters
struct DispersalParams {
	double disp_rate; // adult dispersal rate
	double max_disp; // maximum distance at which two sites are connected (km)
};

// Aestivation model parameters
struct AestivationParams {
	double psi; // aestivation rate
	double mu_aes; // aestivation mortality
	int t_hide1; // start day of aestivation-entering period (day number of the year), not included
	int t_hide2; // end day of aestivation-entering period (day number of the year)
	int t_wake1; // start day of aestivation-waking period (day number of the year), not included
	int t_wake2; // end day of aestivation-waking period (day number of the year)
};

// Seasonality model parameters
struct SineRainfallParams {
    double alpha1; // rainfall contribution factor to carrying capacity
    double amp; // amplitude of rainfall fluctuations
};

struct InputRainfallParams {
	double alpha1; // rainfall contribution factor to carrying capacity
	double resp; // carrying capacity's responsiveness to rainfall contribution
    std::vector<double> rainfall; // daily rainfall for every day of the year
};

// Initial population values for the model
struct InitialPopsParams {
	int initial_WJ; // array of number of initial juvenile mosquitoes with wild homozygous (WW) genotype for each age group
	int initial_WM; // number of initial adult male mosquitoes with wild homozygous (WW) genotype
	int initial_WV; // number of initial adult unmated female (virgin) mosquitoes with wild homozygous (WW) genotype
	int initial_WF; // number of initial adult mated female mosquitoes with wild homozygous (WW) genotype
};

struct ModelParams {
	AreaParams *area;
	InitialPopsParams *initial;
	LifeParams *life;
	AestivationParams *aes;
	DispersalParams *disp; 
	ReleaseParams *rel;
};

// Data-recording parameters
struct RecordParams {
	// recording window and intervals
	int rec_start; // start time for the data recording window (in days) (non-inclusive)
	int rec_end; // end time for the data recording window (in days) (inclusive)
	int rec_interval_global; // time interval for global data recording/output
	int rec_interval_local; // time interval at which to collect/record local data (in days)
	int rec_sites_freq; // fraction of sites to collect local data for (1 is all sites, 10 is 1 in 10 etc)

	// output filename labels
	int set_label; // 'set of runs' index label for output files
};

#endif //PARAMS_H