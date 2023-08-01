#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib> // for exit function
#include <random>  
#include <cmath> 
#include <numeric>  
#include <algorithm> 
#include <cstdio>
#include <chrono>

const int TL = 20; // juvenile development time (egg to adult) expressed as days left till eclosion (eclosion on day 0)
const int NumGen = 6; // number of different genotypes in the mosquito population
// const double PI = 3.14159265; // used to be used in calculating the connectivity weights
// const long long int LONG_MAX=3147483647000;  // had error

void RunNReps(int N); 
void initiate(); 
void UpdateConnec();
void RunMaxT();
std::vector<int> selectDriverSites(int num_driver_sites);
void PutDriverSites(int pat);
void recordCoords(); 
void recordGlobal(int TT); 
void recordLocal(int TT); 
void SitesPopulate();
void OneStep(int day);

// Population processes (controlled from OneStep)

void JuvGetOlder();
void AdultsDie();
void VirginsMate();
void AdultsMove();
void LayEggs();
void JuvEmerge();
void Hide();
void Wake(int day);
void UpdateComp();
void UpdateMate();

void SetFertility();
void CheckCounts(int TT, char ref); // function for debugging - checks if totals are the same as the number in each class in each population

double distance(double U, double x1, double y1, double x2, double y2); // distance between points (assuming toroidal boundaries)

// Random number generator functions

double Random();
int IRandom(int a, int b);
long long int random_poisson(double landa);
long long int random_binomial(long long int N, double p);
std::vector<long long int> random_Multinomial(long long int N, const std::vector<double>& probs);

// Contains the total population numbers over all space (all patches) for different mosquito types
struct totals {
	long long int J[NumGen]; // array of total number of juvenile mosquitoes with each genotype (over all space)
	long long int M[NumGen]; // array of total number of male mosquitoes with each genotype (over all space)
	long long int V[NumGen]; // array of total number of unmated female (virgin) mosquitoes with each genotype (over all space)
	long long int F[NumGen]; // array of total number of mated female mosquitoes with each genotype (over all space)
	long long int CentF[NumGen]; // array of total number of mated female mosquitoes with each genotype in the central simulated area
	long long int JTot; // total number of juvenile mosquitoes of all genotypes (over all space)
	long long int MTot; // total number of male mosquitoes of all genotypes (over all space)
	long long int VTot; // total number of unmated female (virgin) mosquitoes of all genotypes (over all space)
	long long int FTot; // total number of mated female mosquitoes of all genotypes (over all space)
	int CentSqVils; // number of sites in the central simulated square area
};	

// Contains the initial model population and gene drive parameters to start the model simulation
struct initials {
	int NumAdultsWM; // number of initial adult male mosquitoes with wild homozygous (WW) genotype
	int NumAdultsWV; // number of initial adult unmated female (virgin) mosquitoes with wild homozygous (WW) genotype
	int NumAdultsWF; // number of initial adult mated female mosquitoes with wild homozygous (WW) genotype
	int NumJW[TL]; // array of number of initial juvenile mosquitoes with wild homozygous (WW) genotype for each age group
	
	// gene drive initial parameters
	double driver_time; // time to start releasing drive alleles into the mosquito population
	int NumDriver; // number of drive heterozygous (WD) male mosquitoes per release
	int NumDriverSites; // number of gene drive release sites per year

	// data-recording parameters
	int recSitesFreq; // fraction of sites to collect local data for (1 is all sites, 10 is 1 in 10 etc)
};	
		
// Contains the information of a local mosquito population
struct Patch {
	double x; // x-coordinate of the site 
	double y; // y-coordinate of the site
	long long int J[NumGen][TL]; // 2D array of the number of juvenile mosquitoes with each genotype and in each age group in the local site. Age ordered from oldest (0 days left to eclosion) to youngest (TL - 1 days left)
	long long int JTot; // total number of juvenile mosquitoes in the local site
	long long int M[NumGen]; // array of the number of male mosquitoes with each genotype in the local site
	long long int MTot; // total number of male mosquitoes in the local site
	long long int V[NumGen]; // array of total number of unmated female (virgin) mosquitoes with each genotype in the local site
	long long int F[NumGen][NumGen]; // 2D array of the number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j	
	long long int AesF[NumGen][NumGen];	// 2D array of the number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j that have gone into aestivation
	long long int MoveF[NumGen][NumGen]; // 2D array of the number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j that will be dispersing from the local site 	
	long long int MoveM[NumGen]; // array of the number of male mosquitoes with each genotype that will be dispersing from the local site
	long double comp; // density-dependent survival probability per larvae per day from competition (between 0 and 1)
	long double mate_rate; // probability of an unmated (virgin) female mating on a given day

	// for determining connectivities between patches
	std::vector<int> connecIND; // vector of patch indices of those patches that are connected to the selected patch
	std::vector<double> connecW; // vector of patch connection weights of those patches that are connected to the selected patch. Correspond to the respective element in connecIND
	
	int CentSq; // whether the patch is inside (1) the central simulated area or not (0) 
};

// Contains the simulation timekeeping parameters
struct Times {
	// simulation parameters
	int maxT; // maximum simulated time (in days)
	int NumRuns; // number of simulation replicates to run

	// data-recording parameters
	int recfreq; // frequency at which to collect/record local data (in days)
	int recstart; // start time for the data recording window (in days)
	int recend; //  end time for the data recording window (in days)
	int interval; // time interval for global data recording/output
};

// Contains the model parameters
struct Pars {
	// mosquito life-process parameters
	double muJ; // juvenile density independent mortality rate per day
	double muA; // adult mortality rate per day 
	double beta; // parameter that controls mating rate
	double theta; // average egg laying rate of wildtype females (eggs per day)
	double alpha0; // baseline contribution to carrying capacity
	double LarvProbs[TL]; // array of probabililities of juvenile eclosion for different age groups
	int LarvDevMin; // minimum development time for a juvenile (in days)
	double meanTL; // mean juvenile development time (in days)

	// simulation area parameters
	double U; // size of the simulation area (U x U) (km)
	double CentRad; // radius of the central area of simulation (km)
	int NumPat; // number of population sites chosen for the simulation

	// dispersal parameters
	double d; // adult dispersal rate
	double LD; // maximum distance at which two sites are connected (km)

	// aestivation parameters
	double psi; // aestivation rate
	double muAES; // aestivation mortality
	int t_hide1; // start day of aestivation-entering period (day number of the year)
	int t_hide2; // end day of aestivation-entering period (day number of the year)
	int t_wake1; // start day of aestivation-waking period (day number of the year)
	int t_wake2; // end day of aestivation-waking period (day number of the year)

	// gene drive parameters
	double gamma; // rate of r2 allele formation from W/D meiosis
	double xi; // somatic Cas9 expression fitness cost
	double e; // homing rate in females

	double f[NumGen][NumGen][NumGen]; // f_ijk is the fraction of genotype k offspring from mother with genotype i mated to father with genotype j

	// data-recording parameters
	int set; // 'set of runs' index label for output files
	int index; // 'run' index label in given set of runs for output files
};