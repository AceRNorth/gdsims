#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <time.h>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib> // for exit function
#include <tr1/random>
#include <random>
#include <cmath>
#include <numeric>
//#include <random>
#include <chrono>
			
/*-----------------------------------------------------Header definitions---------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/


const int TL=20;//juvenile development time (egg to adult)
const int NumGen=6;

using namespace std;
//using std::ifstream;
using namespace std::tr1;

const double PI=3.14159265;
const double TWOPI=6.28318531;
const long long int LONG_MAX=3147483647000;


/*----------------------struct for keeping track of global numbers------------------------*/
struct totals
	{
	
	long long int J[NumGen];
	long long int M[NumGen];
	long long int V[NumGen];
	long long int F[NumGen];
	long long int CentF[NumGen];
	long long int JTot,MTot,VTot,FTot;
	int CentSqVils;
	double CentSqHum;
	double distW,distD,distR;
	};	
/*----------------------------------------------------------------------------------------*/


/*----------------------struct combining initial condition parameters---------------------*/
struct initials
{
	int NumAdultsWM; int NumAdultsWV; int NumAdultsWF;
	int NumAdultsDM; int NumAdultsDV; int NumAdultsDF;
	int NumAdultsRM; int NumAdultsRV; int NumAdultsRF;
	int NumJW[TL]; int NumJD[TL]; int NumJR[TL];
	double driver_time;
	double driver_start;
	double driver_end;
	double r_time;
	int NumDriver;
	double NumDriverSites;
	double NumDriverD;
	double NumDriverSitesD;
	int NumR;
	int NumRes;
	char dist;
	int recSitesFreq;
	};	
/*----------------------------------------------------------------------------------------*/

		
/*------------------------state of population in each settlement---------------------------*/
struct Patch
{
	double x;
	double y;
//	double Nhum;
	double gam;
	double arab;
	double fun;
	double area;
	double dens;
	int sqx,sqy;
	int numsites;
	long long int J[NumGen][TL];
	long long int JTot;
	long long int MTot;
	long long int M[NumGen];
	long long int V[NumGen];
	//int Fww,Fwd,Fdd,Fwr,Frr,Fdr;
	long long int F[NumGen][NumGen];	
	long long int AesF[NumGen][NumGen];	
	long long int MoveF[NumGen][NumGen];	
	long long int MoveM[NumGen];
	long double comp;
	long double mate_rate;


/*----- for determining connectivities between patches-------*/
	vector<int> connecIND; //connection indices
	vector<double> connecW;// connection weights
	double TotW;// sum of connection weights
/*-----------------------------------------------------------*/

	int CentSq;
	double distOrigin;
};
/*----------------------------------------------------------------------------------------*/



/*----------------struct containt simulation timekeeping parameters-------------------------*/
struct Times
{
	int maxT;// maximum simulated time
	int start;
	int recfreq; //record frequency
	int recstart;// start recording
	int recend;// end recording
	int NumRuns;// number of simulation replicates to run
	int interval;// how often to output global variables
};



/*----------------------------------------------------------------------------------------*/

/*---------------------struct containing model parameters----------------------------------*/
struct Pars
	{
	double muJ,muA,d,gamma,Fgamma,Mgamma,beta,theta,Frho,Mrho,xi,e,em,ef,LD,dL,muLD,psi,muAES,bias;
	double alpha0,alpha1,alpha2,delta,phi,kappa,al0var,mu,sig;
	double omega,s;
	double LarvProbs[TL];
	double meanTL;
	double U;// size of simulation area (side)
	double CentRad;//radius of interior area of simulation
	int kernel;
	int singlepop;
	int NumPat;// number of population sites
	char species;
	int set,index;// for labelling the output files
	/*----- aestivation parameters------------*/
	int t_hide1,t_hide2,t_wake1,t_wake2;
	/*----------------------------------------*/
	double f[NumGen][NumGen][NumGen];//f_ijk is fraction of type k offspring from mother with genotype i mated to father with genotype j
	};
/*----------------------------------------------------------------------------------------*/

/*-------------------------------function declarations-------------------------------------*/
		void RunMaxT();
		void RunNReps(int);
		void ResetSites();
		void initiate();// set up the simulation sites and variables
		void SitesPopulate(int);// initiate populations in the sites
		void record(int);// output data from each individual site
		void PutDriverSites(int); // release gene drives into population
		void UpdateConnec();// computes inter-patch connectivities
		void OneStep(int);// controll population processes

		/*--------- population processes (controlled from OneStep)-----*/
		void JuvGetOlder();
		void VirginsMate();
		void AdultsMove();
		void Hide();
		void Wake(int);
		void LayEggs();
		void JuvEmerge();
		void AdultsDie();
		void UpdateComp(int);
		void UpdateMate();
		/*-------------------------------------------------------------------*/
		void SetFertility();
		void CheckCounts(int,char);// function for debugging - checks if totals are the same as the number in each class in each population

		/*--------random number generater functions-----------------------*/
		long long int random_poisson(double);
		long long int random_binomial(long long int,double);
		int IRandom(int, int);
		double Random();
		double random_normal(double,double);
		std::vector<int> randomizeMultinomial(int n, const std::vector<double>& p);
		/*-------------------------------------------------------------------*/

		double distance(double,double,double,double,double);//distance between points (assuming toroidal boundaries)
		double abso(double);//absolute used in distance function

/*----------------------------------------------------------------------------------------*/
