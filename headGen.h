#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cstdlib> // for exit function
#include <random>  
#include <cmath> 
#include <numeric>  
#include <algorithm> 
#include <cstdio>
#include <chrono>

const int max_dev = 20; // juvenile development time (egg to adult) expressed as days left till eclosion (eclosion on day 0)
const int num_gen = 6; // number of different genotypes in the mosquito population
// const long long int LONG_MAX=3147483647000;

void run_reps(int n); 
void initiate(); 
void set_connec();
void run_model();
std::vector<int> select_driver_sites(int num_driver_sites);
void put_driver_sites(const std::vector<int>& patches);
void record_coords(); 
void record_global(int day); 
void record_local(int day); 
void populate_sites();
void run_step(int day);

// Population processes (controlled from run_step)

void juv_get_older();
void adults_die();
void virgins_mate();
void adults_move();
void lay_eggs();
void juv_emerge();
void hide();
void wake(int day);
void update_comp();
void update_mate();

void set_inheritance();
void check_counts(int day, char ref); 

double distance(double side, std::array<double, 2> point1, std::array<double, 2> point2);

// Random number generator functions

double random_real();
int random_discrete(int a, int b);
long long int random_poisson(double lambda);
long long int random_binomial(long long int n, double p);
std::vector<long long int> random_multinomial(long long int n, const std::vector<double>& probs);

// Contains the total population numbers over all space (all patches) for different mosquito types
struct Totals {
	std::array<long long int, num_gen> J; // array of total number of juvenile mosquitoes with each genotype (over all space)
	std::array<long long int, num_gen> M; // array of total number of male mosquitoes with each genotype (over all space)
	std::array<long long int, num_gen> V; // array of total number of unmated female (virgin) mosquitoes with each genotype (over all space)
	std::array<long long int, num_gen> F; // array of total number of mated female mosquitoes with each genotype (over all space)
	long long int tot_J; // total number of juvenile mosquitoes of all genotypes (over all space)
	long long int tot_M; // total number of male mosquitoes of all genotypes (over all space)
	long long int tot_V; // total number of unmated female (virgin) mosquitoes of all genotypes (over all space)
	long long int tot_F; // total number of mated female mosquitoes of all genotypes (over all space)
	int central_sites; // number of sites in the central simulated square area
};	

// Contains the initial model population and gene drive parameters to start the model simulation
struct Initials {
	int initial_WM; // number of initial adult male mosquitoes with wild homozygous (WW) genotype
	int initial_WV; // number of initial adult unmated female (virgin) mosquitoes with wild homozygous (WW) genotype
	int initial_WF; // number of initial adult mated female mosquitoes with wild homozygous (WW) genotype
	std::array<int, max_dev> initial_WJ; // array of number of initial juvenile mosquitoes with wild homozygous (WW) genotype for each age group
	
	// gene drive initial parameters
	double driver_start; // time to start releasing drive alleles into the mosquito population
	int num_driver_M; // number of drive heterozygous (WD) male mosquitoes per release
	int num_driver_sites; // number of gene drive release sites per year

	// data-recording parameters
	int rec_sites_freq; // fraction of sites to collect local data for (1 is all sites, 10 is 1 in 10 etc)
};	
		
// Contains the information of a local mosquito population
struct Patch {
	std::array<double, 2> coords; // (x, y) coordinates of the site 
	std::array<std::array<long long int, max_dev>, num_gen> J; // 2D array of the number of juvenile mosquitoes with each genotype and in each age group in the local site. Age ordered from oldest (0 days left to eclosion) to youngest (TL - 1 days left)
	long long int tot_J; // total number of juvenile mosquitoes in the local site
	std::array<long long int, num_gen> M; // array of the number of male mosquitoes with each genotype in the local site
	long long int tot_M; // total number of male mosquitoes in the local site
	std::array<long long int, num_gen> V; // array of total number of unmated female (virgin) mosquitoes with each genotype in the local site
	std::array<std::array<long long int, num_gen>, num_gen> F; // 2D array of the number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j	
	std::array<std::array<long long int, num_gen>, num_gen> aes_F;	// 2D array of the number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j that have gone into aestivation
	std::array<std::array<long long int, num_gen>, num_gen> move_F; // 2D array of the number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j that will be dispersing from the local site 	
	std::array<long long int, num_gen> move_M; // array of the number of male mosquitoes with each genotype that will be dispersing from the local site
	long double comp; // density-dependent survival probability per larvae per day from competition (between 0 and 1)
	long double mate_rate; // probability of an unmated (virgin) female mating on a given day

	// for determining connectivities between patches
	std::vector<int> connec_indices; // vector of patch indices of those patches that are connected to the selected patch
	std::vector<double> connec_weights; // vector of patch connection weights of those patches that are connected to the selected patch. Correspond to the respective element in connecIND
	
	bool is_central; // whether the patch is inside (1) the central simulated area or not (0) 
};

// Contains the simulation timekeeping parameters
struct Times {
	// simulation parameters
	int max_t; // maximum simulated time (in days)
	int num_reps; // number of simulation replicates to run

	// data-recording parameters
	int rec_interval_local; // time interval at which to collect/record local data (in days)
	int rec_start; // start time for the data recording window (in days)
	int rec_end; //  end time for the data recording window (in days)
	int rec_interval_global; // time interval for global data recording/output
};

// Contains the model parameters
struct Pars {
	// mosquito life-process parameters
	double mu_j; // juvenile density independent mortality rate per day
	double mu_a; // adult mortality rate per day 
	double beta; // parameter that controls mating rate
	double theta; // average egg laying rate of wildtype females (eggs per day)
	double alpha0; // baseline contribution to carrying capacity
	std::array<double, max_dev> eclosion_probs; // array of probabililities of juvenile eclosion for different age groups
	int min_dev; // minimum development time for a juvenile (in days)
	double mean_dev; // mean juvenile development time (in days)

	// simulation area parameters
	double side; // size of the simulation area (side x side) (km)
	double central_radius; // radius of the central area of simulation (km)
	int num_pat; // number of population sites chosen for the simulation

	// dispersal parameters
	double disp_rate; // adult dispersal rate
	double max_disp; // maximum distance at which two sites are connected (km)

	// aestivation parameters
	double psi; // aestivation rate
	double mu_aes; // aestivation mortality
	int t_hide1; // start day of aestivation-entering period (day number of the year)
	int t_hide2; // end day of aestivation-entering period (day number of the year)
	int t_wake1; // start day of aestivation-waking period (day number of the year)
	int t_wake2; // end day of aestivation-waking period (day number of the year)

	// gene drive parameters
	double gamma; // rate of r2 allele formation from W/D meiosis
	double xi; // somatic Cas9 expression fitness cost
	double e; // homing rate in females

	std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> f; // f_ijk is the fraction of genotype k offspring from mother with genotype i mated to father with genotype j

	// data-recording parameters
	int set_label; // 'set of runs' index label for output files
	int run_label; // 'run' index label in given set of runs for output files
};