#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <random>  
#include <cmath> 
#include <numeric>  
#include <algorithm> 
#include <cstdio>
#include <chrono>

const int max_dev = 20; // juvenile development time (egg to adult) expressed as days left till eclosion (eclosion on day 0)
const int num_gen = 6; // number of different genotypes in the mosquito population

// Random number generator functions

double random_real();
int random_discrete(int a, int b);
long long int random_poisson(double lambda);
long long int random_binomial(long long int n, double p);
std::vector<long long int> random_multinomial(long long int n, const std::vector<double>& probs);
std::vector<long long int> random_multinomial(long long int n, const std::array<long long int, num_gen>& probs);
std::vector<long long int> random_multinomial(long long int n, const std::array<double, max_dev+1>& probs);

// Model progression parameters
struct ProgressionParams {
	const int num_runs; // number of simulation replicates to run
	const int max_t; // maximum simulated time (in days)
};

// Model area parameters
struct AreaParams {
	const int num_pat; // number of population sites chosen for the simulation
	const double side; // size of the square simulation area (side x side) (km)
};

// Model life-process parameters
struct LifeParams {
	const double mu_j; // juvenile density independent mortality rate per day
	const double mu_a; // adult mortality rate per day
	const double beta; // parameter that controls mating rate
	const double theta; // average egg laying rate of wildtype females (eggs per day)
	const double alpha0; // baseline contribution to carrying capacity
	const double mean_dev; // mean juvenile development time (in days)
	const int min_dev; // minimum development time for a juvenile (in days)
};

// Gene drive inheritance parameters
struct InheritanceParams {
	const double gamma; // rate of r2 allele formation from W/D meiosis
	const double xi; // somatic Cas9 expression fitness cost
	const double e; // homing rate in females
};

// Gene drive release model parameters
struct ReleaseParams {
	const int driver_start; // time to start releasing drive alleles into the mosquito population
	const int num_driver_M; // number of drive heterozygous (WD) male mosquitoes per release
	const int num_driver_sites; // number of gene drive release sites per year
};

// Dispersal model parameters
struct DispersalParams {
	const double disp_rate; // adult dispersal rate
	const double max_disp; // maximum distance at which two sites are connected (km)
};

// Aestivation model parameters
struct AestivationParams {
	const double psi; // aestivation rate
	const double mu_aes; // aestivation mortality
	const int t_hide1; // start day of aestivation-entering period (day number of the year), not included
	const int t_hide2; // end day of aestivation-entering period (day number of the year)
	const int t_wake1; // start day of aestivation-waking period (day number of the year), not included
	const int t_wake2; // end day of aestivation-waking period (day number of the year)
};

// Initial population values for the model
struct InitialPopsParams {
	const int initial_WJ = 10000; // array of number of initial juvenile mosquitoes with wild homozygous (WW) genotype for each age group
	const int initial_WM = 50000; // number of initial adult male mosquitoes with wild homozygous (WW) genotype
	const int initial_WV = 10000; // number of initial adult unmated female (virgin) mosquitoes with wild homozygous (WW) genotype
	const int initial_WF = 40000; // number of initial adult mated female mosquitoes with wild homozygous (WW) genotype
};

// Data-recording parameters
struct RecordParams {
	// recording window and intervals
	const int rec_start; // start time for the data recording window (in days) (non-inclusive)
	const int rec_end; // end time for the data recording window (in days) (inclusive)
	const int rec_interval_global; // time interval for global data recording/output
	const int rec_interval_local; // time interval at which to collect/record local data (in days)
	const int rec_sites_freq; // fraction of sites to collect local data for (1 is all sites, 10 is 1 in 10 etc)

	// output filename labels
	const int set_label; // 'set of runs' index label for output files
};

// Sets up and controls the flow of the simulation.
class SimController {
public:
	SimController(ProgressionParams &prog, AreaParams &area, LifeParams &life, InheritanceParams &inher,
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
	std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> f;
};

class Patch;
class Dispersal;
class Aestivation;
class GDRelease;

// Runs the model.
class Model {
public:
	Model(AreaParams *area, InitialPopsParams *initial, LifeParams *life, AestivationParams *aes, DispersalParams *disp_param, 
		ReleaseParams *rel);
	~Model();
	void initiate();
	void run(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f);
	void run_step(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f);

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
	void juv_get_older();
	void adults_die();
	void virgins_mate();
	void lay_eggs(const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f);
	void juv_eclose();
	void update_comp();
	void update_mate();

};

// Contains the information of a local mosquito population
class Patch {
public:
	Patch(double side, LifeParams* params);
	void populate(int initial_WJ, int initial_WM, int initial_WV, int initial_WF);

	std::array<double, 2> get_coords() const;
	std::array<long long int, num_gen> get_M() const;
	std::array<std::array<long long int, num_gen>, num_gen> get_F() const;

	long long int calculate_tot_J();
	long long int calculate_tot_M();
	long long int calculate_tot_V();
	long long int calculate_tot_F();

	// life-processes for the local site
	void juv_get_older(int max_dev);
	void adults_die(double mu_a);
	void virgins_mate();
	void lay_eggs(const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f, 
		double theta, const std::array<double, max_dev+1> &dev_duration_probs);
	void juv_eclose();
	void update_comp(double mu_j, double alpha0, double mean_dev);
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

	std::array<double, 2> coords; // (x, y) coordinates of the site
	// number of juvenile mosquitoes with each genotype and in each age group.
	// Age ordered from oldest (0 days left to eclosion) to youngest (TL - 1 days left)
	std::array<std::array<long long int, max_dev+1>, num_gen> J; 
	std::array<long long int, num_gen> M; // number of male mosquitoes with each genotype
	std::array<long long int, num_gen> V; // number of unmated female (virgin) mosquitoes with each genotype
	// number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j	
	std::array<std::array<long long int, num_gen>, num_gen> F; 

	long double comp; // survival probability per juvenile per day (both density-dependent and independent factors)
	long double mate_rate; // probability of an unmated (virgin) female mating on a given day
};

// Implement aestivative behaviour for the collection of mosquito sites. 
class Aestivation {
public:
	Aestivation(AestivationParams *params, int sites_size);
	void hide(std::vector<Patch*> &sites);
	void wake(int day, std::vector<Patch*> &sites);
	bool is_hide_time(int day);
	bool is_wake_time(int day);

private:
	double psi; // aestivation rate
	double mu_aes; // aestivation mortality
	int t_hide1; // start day of aestivation-entering period (day number of the year), not included
	int t_hide2; // end day of aestivation-entering period (day number of the year)
	int t_wake1; // start day of aestivation-waking period (day number of the year), not included
	int t_wake2; // end day of aestivation-waking period (day number of the year)

	// number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j that have gone into
	// aestivation from each patch
	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> aes_F;
};

// Implements dispersion across all mosquito sites in the collection. 
class Dispersal {
public:
	Dispersal(DispersalParams* params);
	void set_connecs(double side, std::vector<Patch*> &sites);
	void adults_disperse(std::vector<Patch*> &sites);

private:
	double disp_rate; // adult dispersal rate
	double max_disp; // maximum distance at which two sites are connected (km)

	// connected patch indices ordered by each patch in sites, such that the first element contains the indices of all the patches
	// connected to the first sites patch, second element has all connection indices to the second sites patch, etc.
	std::vector<std::vector<int>> connec_indices; 
	// connection weights of the connected patches ordered by each patch in sites, such that the first element contains the connection
	// weights between the first patch in sites and all the patches connected to it, the second element has all connection weights
	// between the second sites element and all other patches connected to it, etc.
	std::vector<std::vector<double>> connec_weights; 

	double distance(double side, std::array<double, 2> point1, std::array<double, 2> point2);
	std::vector<std::array<long long int, num_gen>> M_dispersing_out(const std::vector<Patch*> &sites);
	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> F_dispersing_out(const std::vector<Patch*> &sites);
};

// Implements gene drive release of mosquitoes into the collection of mosquito sites.
class GDRelease {
public:
	GDRelease(ReleaseParams* params);
	void release_gene_drive(std::vector<Patch*> &sites);
	bool is_release_time(int day);

private:
	int driver_start; // time to start releasing drive alleles into the mosquito population
	int num_driver_M; // number of drive heterozygous (WD) male mosquitoes per release
	int num_driver_sites; // number of gene drive release sites per year

	std::vector<Patch*> select_driver_sites(int num_rel_sites, const std::vector<Patch*> &sites);
	void put_driver_sites(std::vector<Patch*>& rel_sites, std::vector<Patch*> &sites);
};

// Records model data.
class Record {
public:
	Record(RecordParams *rec_params, int rep);
	void record_coords(const std::vector<Patch*> &sites);
	void record_global(int day, const std::array<long long int, num_gen> &tot_M_gen);
	void output_totals(int day, long long int tot_J, long long int tot_M, long long int tot_V, long long int tot_F);
	void record_local(int day, const std::vector<Patch*> &sites);

	bool is_rec_local_time(int day);
	bool is_rec_global_time(int day);

private:
	// recording window and intervals
	int rec_start; // start time for the data recording window (in days) (non-inclusive)
	int rec_end; // end time for the data recording window (in days) (inclusive)
	int rec_interval_global; // time interval for global data recording/output
	int rec_interval_local; // time interval at which to collect/record local data (in days)
	int rec_sites_freq; // fraction of sites to collect local data for (1 is all sites, 10 is 1 in 10 etc)

	// output filename labels
	int set_label; // 'set of repetitions' index label for output files
	int rep_label; // 'repetition' index label in given set of repetitions for output files

	std::ostringstream os1, os2, os3; // filenames
	std::ofstream local_data, global_data, coord_list; // file objects
};

// Gives the reason for invalid input.
class Exception {
public:
    Exception();
    virtual void message() = 0;
};

class OutOfBoundsException: public Exception {
public:
    OutOfBoundsException(const std::string& param);
    void message();

private:
    std::string par; // parameter that caused the exception
};

class InvalidIntervalException: public Exception {
public:
    InvalidIntervalException(const std::string& param1, const std::string& param2);
    void message();

private:
    std::string inter1; // start of the interval
    std::string inter2; // end of the interval
	
};