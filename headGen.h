#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime> // for runtime calculations
#include <cstdlib> // for exit function
#include <random>  
#include <cmath> 
#include <numeric>  
#include <algorithm> 

/*-----------------------------------------------------Header definitions---------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
const int TL = 20; // juvenile development time (egg to adult)
const int NumGen = 6;

// const double PI = 3.14159265;
// const double TWOPI = 6.28318531;
// const long long int LONG_MAX=3147483647000;  // had error - figure what's going wrong

/*-------------------------------function declarations-------------------------------------*/
void RunMaxT();
void RunNReps(int N);
void initiate(); // set up the simulation sites and variables
void SitesPopulate(int pat); // initiate populations in the sites
void record(); // output data from each individual site
void PutDriverSites(int pat); // release gene drives into population
void UpdateConnec(); // computes inter-patch connectivities
void OneStep(int day); // control population processes

/*--------- population processes (controlled from OneStep)-----*/
void JuvGetOlder();
void VirginsMate();
void AdultsMove();
void Hide();
void Wake(int day);
void LayEggs();
void JuvEmerge();
void AdultsDie();
void UpdateComp(int day);
void UpdateMate();
/*-------------------------------------------------------------------*/
void SetFertility();
void CheckCounts(int TT, char ref); // function for debugging - checks if totals are the same as the number in each class in each population

/*--------random number generater functions-----------------------*/
long long int random_poisson(double landa);
long long int random_binomial(long long int N, double p);
int IRandom(int a, int b);
double Random();
// double random_normal(double,double);
std::vector<int> random_Multinomial(int N, const std::vector<double>& probs);
/*-------------------------------------------------------------------*/

double distance(double U, double x1, double y1, double x2, double y2); // distance between points (assuming toroidal boundaries)
double abso(double XX); // absolute used in distance function


/*----------------------struct for keeping track of global numbers------------------------*/
struct totals {
	long long int J[NumGen];
	long long int M[NumGen];
	long long int V[NumGen];
	long long int F[NumGen];
	long long int CentF[NumGen];
	long long int JTot, MTot, VTot, FTot;
	int CentSqVils;
	double CentSqHum;
};	

/*----------------------struct combining initial condition parameters---------------------*/
struct initials {
	int NumAdultsWM, NumAdultsWV, NumAdultsWF;
	// int NumAdultsDM, NumAdultsDV, NumAdultsDF;
	// int NumAdultsRM, NumAdultsRV, NumAdultsRF;
	int NumJW[TL];
	// int NumJD[TL];
	// int NumJR[TL];
	double driver_time;
	double driver_start;
	double driver_end;
	// double r_time;
	int NumDriver;
	double NumDriverSites;
	// double NumDriverD;
	// double NumDriverSitesD;
	// int NumR;
	// int NumRes;
	int recSitesFreq;
};	
		
/*------------------------state of population in each settlement---------------------------*/
struct Patch {
	double x;
	double y;
	// double Nhum;
	double gam;
	double arab;
	double fun;
	double area;
	// double dens;
	int sqx, sqy;
	// int numsites;
	long long int J[NumGen][TL];
	long long int JTot;
	long long int M[NumGen];
	long long int MTot;
	long long int V[NumGen];
	// int Fww, Fwd, Fdd, Fwr, Frr, Fdr;
	long long int F[NumGen][NumGen];	
	long long int AesF[NumGen][NumGen];	
	long long int MoveF[NumGen][NumGen];	
	long long int MoveM[NumGen];
	long double comp; // survival probability per larvae per day from competition (between 0 and 1)
	long double mate_rate; // probability a virgin female mates on a given day

	/*----- for determining connectivities between patches-------*/
	std::vector<int> connecIND; // connection indices
	std::vector<double> connecW; // connection weights
	double TotW; // sum of connection weights
	/*-----------------------------------------------------------*/
	int CentSq;
};

/*----------------struct containt simulation timekeeping parameters-------------------------*/
struct Times {
	int maxT; // maximum simulated time
	int start;
	int recfreq; // record frequency
	int recstart; // start recording
	int recend; // end recording
	int NumRuns; // number of simulation replicates to run
	int interval; // how often to output global variables
};

/*---------------------struct containing model parameters----------------------------------*/
struct Pars {
	double muJ;
	double muA;
	double d;
	double gamma;
	double beta;
	double theta;
	double xi;
	double e;
	// double em;
	// double ef;
	// double dL;
	// double muLD;
	double psi;
	double muAES;
	double LD; // maximum distance at which two sites are connected
	double alpha0;
	double LarvProbs[TL];
	double meanTL;
	double U; // size of simulation area (side)
	double CentRad; // radius of interior area of simulation
	int NumPat; // number of population sites
	int set, index; // for labelling the output files

	/*----- aestivation parameters------------*/
	int t_hide1,t_hide2,t_wake1,t_wake2;
	/*----------------------------------------*/

	double f[NumGen][NumGen][NumGen]; // f_ijk is fraction of type k offspring from mother with genotype i mated to father with genotype j
};