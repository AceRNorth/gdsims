#include "headGen.h"

// Global variables

// Random number seed
std::random_device rd;
std::mt19937 gen(rd());

// Global structs
Pars pa; // parameters 
initials in; // initial conditions
Times ti; // timekeeping parameters
totals to; // totals
std::vector<Patch> Site; // information on each population

// Output files
std::ostringstream os1, os2, os3;
std::ofstream localinfo, globalinfo, ParList;
//	clock_t now=clock();

int main()
{
	// Input parameters
	std::cin >> pa.set; // enumeration for output files
	std::cin >> pa.index;
	std::cin >> pa.NumPat; // number of sites
	std::cin >> in.recSitesFreq; // if outputting local data, how many sites to collect for (1 is all sites, 10 is 1 in 10 etc)
	std::cin >> ti.interval; // time interval for outputting global data
	std::cin >> ti.start;
	std::cin >> ti.maxT; // maximum time to simulate until (days)
	std::cin >> ti.recfreq; // how often to collect local data (every rec days)
	std::cin >> ti.recstart; // how often to collect local data (every rec days)
	std::cin >> ti.recend; // how often to collect local data (every rec days)
	std::cin >> ti.NumRuns; // how many simulation runs
	std::cin >> in.driver_time; // time to start releasing drive alleles; if negative, the release dates are random, if positive, the releases are on the specific date
	std::cin >> in.driver_end; // time to end releasing drive alleles
	std::cin >> in.NumDriver; // number of drive homozygous male mosquitoes per release
	std::cin >> in.NumDriverSites; // number of release sites per year
	std::cin >> pa.muJ; // juvenile density independent mortality per day
	std::cin >> pa.muA; // adult mortality per day
	std::cin >> pa.d; // adult dispersal rate
	std::cin >> pa.gamma; // rate of r2 allele formation from W/D meiosis
	std::cin >> pa.beta; // parameter that controls mating rate
	std::cin >> pa.theta; // egg laying rate of wildtype females (eggs per day)
	std::cin >> pa.xi; // somatic Cas9 expression fitness cost
	std::cin >> pa.e; // homing rate in females
	std::cin >> pa.LD; // maximum distance at which settlements are connected

	std::cin >> pa.psi; // Aestivation rate
	std::cin >> pa.muAES; // aestivation mortality
	std::cin >> pa.t_hide1; // start day of going into aestivation
	std::cin >> pa.t_hide2; // end day of going into aestivation
	std::cin >> pa.t_wake1; // start day of emerging from aestivation
	std::cin >> pa.t_wake2; // end day of emerging from aestivation
	std::cin >> pa.alpha0; // baseline contribution to carrying capacity
	std::cin >> pa.meanTL;
	std::cin >> pa.LarvDevMin;

	std::cin >> pa.U;
	std::cin >> pa.CentRad;
	// std::cout << pa.U << "   " << pa.muA << "  " << pa.muJ << std::endl;

	// setting probabilities of juvenile eclosion for different age groups
	for (int age=0; age<TL; age++) {
        if (age >= pa.LarvDevMin) {
            pa.LarvProbs[age] = 1.0 / (TL - pa.LarvDevMin);
        }
        else {
            pa.LarvProbs[age] = 0;
        }
    }

	// Initial populations
	in.NumAdultsWV = 10000;
	in.NumAdultsWM = 50000;
	in.NumAdultsWF = 40000;

	for (int a=0; a<TL; a++) {
		in.NumJW[a] = 10000;
	}

	// Set inheritance architecture
	SetFertility(); // only used in LayEggs()!

	// Run model NumRuns times
	RunNReps(ti.NumRuns);
	
	return 0;
}
	
	
void RunNReps(int N) {
	// place new function here to create new files? but will need to close them later on
	os1 << "LocalData" << pa.set << "run" << pa.index << ".txt"; // make a file for outputing local data
	localinfo.open(os1.str().c_str());
	os2 << "Totals" << pa.set << "run" << pa.index << ".txt"; // make a file for outputing global data
	globalinfo.open(os2.str().c_str());
	os3 << "CoordinateList" << pa.set << "run" << pa.index << ".txt";
	ParList.open(os3.str().c_str());

	std::clock_t start;
	double dtime;
	start = std::clock();

	for (int j=0; j<N; j++) {
		initiate();

		// place a new recordPosition()/recordPatches() function?
		for (int pat=0; pat < Site.size(); pat += in.recSitesFreq) {
			ParList << Site[pat].x << "\t" << Site[pat].y << std::endl;
		}

		RunMaxT();
	}

	dtime = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	os1.str(""); // why?
	localinfo.close();
	os2.str(""); 
	globalinfo.close();
	os3.str("");
	ParList.close();
}

void initiate() {
	for (int i=0; i<NumGen; i++) {
		to.J[i] = 0;
		to.M[i] = 0;
		to.V[i] = 0;
		to.F[i] = 0;
		to.CentF[i] = 0;
	}

	to.JTot = 0, to.MTot = 0, to.VTot = 0, to.FTot = 0;
	to.CentSqVils = 0;
	Site.clear();
	Patch pp;
	// cout << "init   1" << endl;
	
	// Input the settlement data
	for (int ii=0; ii < pa.NumPat; ii++) {       
		pp.x = Random() * pa.U;
		pp.y = Random() * pa.U;
		pp.CentSq = 0;

		if (distance(pa.U, pp.x, pp.y, pa.U / 2.0, pa.U / 2.0) < pa.CentRad) {
			pp.CentSq = 1;
		}

		pp.connecIND.clear();
		pp.connecW.clear();

		for (int i=0; i<NumGen; i++) {
			for (int a=0; a<TL; a++) {
				pp.J[i][a] = 0; 
			}

			pp.M[i] = 0;
			pp.V[i] = 0;

			for (int j=0; j<NumGen; j++) {
				pp.F[i][j] = 0;
				pp.AesF[i][j] = 0;
			}
		}
		pp.comp = 0, pp.mate_rate = 0, pp.JTot = 0, pp.MTot = 0;

		if (pp.CentSq == 1) {
			to.CentSqVils++;
		}

		Site.push_back(pp);
	}

	for (int i=0; i < Site.size(); i++) {
		if (Site[i].CentSq == 1) {
			SitesPopulate(i);
		}
	}

	UpdateConnec();
//	CheckCounts(0,'i');
// 	std::cout << "finished initialising" << std::endl;
}

// rename function as FindConnec()/SetConnec()? only being called at start to set connection values
void UpdateConnec() {
	double dd, ww;
	for (int index = 0; index < Site.size(); index++) {
		Site[index].connecIND.clear();
		Site[index].connecW.clear();
		for (int ii=0; ii < Site.size(); ii++) {
		// re-activate this if loop to exclude self-dispersal. need to check isolated sites catered for
		//	if (ii != index) {
				dd = distance(pa.U, Site[index].x, Site[index].y, Site[ii].x, Site[ii].y);
				if (dd < pa.LD) {
					Site[index].connecIND.push_back(ii); 
					//ww = (1 - dd / pa.LD) * (3 / (PI * pa.LD * pa.LD)) / (1.0 * pa.NumPat);
					ww = pa.LD - dd;
					Site[index].connecW.push_back(ww); 
				}
		//	}
		}
	}
}

void RunMaxT() {
	int TT = ti.start;
	int uniquepat, relpat, num_release_sites; 
	
	// Set gene drive release sites
	num_release_sites = std::min(to.CentSqVils, int(in.NumDriverSites));
	int relpatches[num_release_sites]; // patches in which to release the gene drive (contains indices to the patches?)
	int reltimes[num_release_sites]; // release times
	
	for (int jj=0; jj < num_release_sites; jj++) {
		relpatches[jj] = -1; // initialise elements outside the range of possible values, to prevent interference
	}

	for (int jj=0; jj < num_release_sites; jj++) {
		uniquepat = 0;

		while (uniquepat == 0) {
			uniquepat = 1;
			relpat = IRandom(0, Site.size() - 1);

			for (int ii=0; ii < num_release_sites; ii++) {
				if (relpat == relpatches[ii] || Site[relpat].CentSq == 0) {
					uniquepat = 0;
				}
			}
			relpatches[jj] = relpat;
		}

		reltimes[jj] = in.driver_time;
	}
	
	while (TT < ti.maxT + 1) {
		// outputting
		if (TT % ti.interval == 0) {
			// outputs and records total number of each type of mosquito
			std::cout << TT << "\t" << to.JTot << "\t" << to.MTot << "\t" << to.VTot << "\t" << to.FTot << std::endl;
			globalinfo << TT;
			
			// records total number of males of each genotype
			for (int i=0; i<NumGen; i++) {
				globalinfo << "\t" << to.M[i];
			}

			globalinfo << std::endl;
		}

		if (TT > ti.recstart && TT <= ti.recend && TT%ti.recfreq == 0) {
			record();
		}
		
		for (int jj=0; jj < num_release_sites; jj++) {
			if (TT == reltimes[jj]){ // if condition should be outside for loop to speed it up?? especially cause release times for all patches are set to same constant right now
				// gene drive release
				PutDriverSites(relpatches[jj]);
			}
		}

		OneStep(TT);
		TT++;
	}
}

void record() {	
	for(int pat=0; pat < Site.size(); pat += in.recSitesFreq) {
		for(int i=0; i < NumGen - 1; i++) {
			// potentially join lines into one statement - correct this because not doing what it should be doing
			localinfo << Site[pat].M[i] << "    ";
		}
		localinfo << Site[pat].M[5] << std::endl;
	}
}

void PutDriverSites(int pat) {
	Site[pat].M[1] += in.NumDriver;
	Site[pat].MTot += in.NumDriver;
	to.M[1] += in.NumDriver;
	to.MTot += in.NumDriver;
	UpdateMate();
}

void SitesPopulate(int pat) {
	for (int a=0; a<TL; a++) {
		Site[pat].J[0][a] += in.NumJW[a];
		to.J[0] += in.NumJW[a];
		to.JTot += in.NumJW[a];
		Site[pat].JTot += in.NumJW[a];
	}
		
	//	nb can remove int() from below
	Site[pat].M[0] = int(in.NumAdultsWM);
	to.M[0] += int(in.NumAdultsWM);
	to.MTot += int(in.NumAdultsWM);
	Site[pat].MTot += int(in.NumAdultsWM);

	Site[pat].V[0] = int(in.NumAdultsWV);
	to.V[0] += int(in.NumAdultsWV);
	to.VTot += int(in.NumAdultsWV);

	Site[pat].F[0][0] = int(in.NumAdultsWF);
	to.F[0] += int(in.NumAdultsWF);
	to.FTot += int(in.NumAdultsWF);

	// change this to UpdateComp() and UpdateMate() once i make this function apply to all patches in a for loop (but include at the top the inside square condition)
	Site[pat].comp = std::pow((double)pa.alpha0 / (pa.alpha0 + Site[pat].JTot), 1.0 / pa.meanTL);
	Site[pat].mate_rate = Site[pat].MTot / (pa.beta + Site[pat].MTot);
}

void OneStep(int day) {
	//std::cout << "os1  " << to.JTot << "   " << to.MTot << "    " << to.VTot << "   " << to.FTot << std::endl;
	//CheckCounts(0,'o');
	JuvGetOlder();
	AdultsDie();
	VirginsMate();
	AdultsMove();
	LayEggs();
	JuvEmerge();
	if (day%365 > pa.t_hide1 && day%365 <= pa.t_hide2 && pa.psi > 0.00001) Hide();
	if (day%365 > pa.t_wake1 && day%365 <= pa.t_wake2 && pa.psi > 0.00001) Wake(day);
	UpdateComp();
	UpdateMate();
}

void JuvGetOlder() {
	long int jtSitesch, jtAllAll;
	jtAllAll = 0;
	long int jtAll[NumGen];
	for (int i=0; i<NumGen; i++) {
		jtAll[i] = 0; 
	}

	double pcomp;
	for (int pat=0; pat < Site.size(); pat++) {
		pcomp = (1 - pa.muJ) * Site[pat].comp; // probability of survival per larva
		jtSitesch = 0;

		for (int age=0; age < TL - 1; age++) { // check this loop, why < TL - 1 ? and check it works with countdown age notation. if true, need to put in if statement to not age the oldest ones (age 0)
			for (int i=0; i<NumGen; i++) {		
				Site[pat].J[i][age] = random_binomial(Site[pat].J[i][age+1], pcomp); // increment age by 1 (but not decrease since it's countdown?) taking mortality into account (pcomp is survival per larva)
				jtSitesch += Site[pat].J[i][age];
				jtAll[i] += Site[pat].J[i][age];
			}
		}
		
		Site[pat].JTot = jtSitesch;
		jtAllAll += jtSitesch;

		// youngest ones have all aged by one day so 0 left in this age group
		for (int i=0; i<NumGen; i++) {
			Site[pat].J[i][TL-1] = 0;
		} 
	}

	for (int i=0; i<NumGen; i++) {
		to.J[i] = jtAll[i];
	}

	to.JTot = jtAllAll;
}

void AdultsDie() {
	long long int num;
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++) {
			num = random_binomial(Site[pat].M[i], pa.muA); // number that die
			Site[pat].M[i] -= num;
			Site[pat].MTot -= num;
			to.M[i] -= num;
			to.MTot -= num;	

			num = random_binomial(Site[pat].V[i], pa.muA);
			Site[pat].V[i] -= num;
			to.V[i] -= num;
			to.VTot -= num;	

			for (int j=0; j<NumGen; j++) {
				num = random_binomial(Site[pat].F[i][j], pa.muA);
				Site[pat].F[i][j] -= num;
				to.F[i] -= num;
				to.FTot -= num;	
			}
		}
	}
}

void VirginsMate() {
	int v[NumGen];
	std::vector<double> m(NumGen);
	std::vector<int> randomCounts;
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++) {
			m[i] = (1.0 * Site[pat].M[i]); // why the multiplication factor ?) there might also be better way of using this global data instead of creating duplicate local variable. maybe use pointers?? 
		}

		for (int i=0; i<NumGen; i++) {
			v[i] = random_binomial(Site[pat].V[i], Site[pat].mate_rate); // how many V will mate

			if (v[i] > 0) {
				// vector of number of mated females carrying each male genotype (?)
				randomCounts = random_Multinomial(v[i], m); // with m and not m[i] ? why? what is randomCounts?

				for (int j=0; j<NumGen; j++) {
					Site[pat].F[i][j] += randomCounts[j];
				}

				Site[pat].V[i] -= v[i];
				to.F[i] += v[i];
				to.V[i] -= v[i];
				to.VTot -= v[i];
				to.FTot += v[i];
			}
		}
	}
}

void AdultsMove() {
	int pat;
	std::vector<int> randomCounts;
	if (Site.size() > 1) {
		// number of adults dispersing from each patch (with each genotype)
		for (pat=0; pat < Site.size(); pat++) {
			for (int i=0; i<NumGen; i++) {
				Site[pat].MoveM[i] = random_binomial(Site[pat].M[i], pa.d); // how many males will disperse
				Site[pat].M[i] -= Site[pat].MoveM[i];
				Site[pat].MTot -= Site[pat].MoveM[i];

				for (int j=0; j<NumGen; j++) {
					Site[pat].MoveF[i][j] = random_binomial(Site[pat].F[i][j], pa.d); // how many (mated) females will disperse
					Site[pat].F[i][j] -= Site[pat].MoveF[i][j]; 
					// not updating female total here too? no variable for that in Patch?
				}
			}
		}

		// number of adults dispersing to each patch (with each genotype)
		for (pat=0; pat < Site.size(); pat++) {
			for (int i=0; i<NumGen; i++) {
				randomCounts = random_Multinomial(Site[pat].MoveM[i], Site[pat].connecW); // how many males will disperse to each connected patch
				
				for (int newpat=0; newpat < randomCounts.size(); newpat++) {
					Site[Site[pat].connecIND[newpat]].M[i] += randomCounts[newpat];
					// not updating totals? can't subtract in previous loop and then not add them back
				}

				for (int j=0; j<NumGen; j++) {
					if (Site[pat].MoveF[i][j] > 0) {
						//std::vector<int> randomCounts = random_Multinomial(Site[pat].MoveF[i][j], Site[pat].connecW);
						randomCounts = random_Multinomial(Site[pat].MoveF[i][j], Site[pat].connecW);

						for (int newpat=0; newpat < randomCounts.size(); newpat++) {
							Site[Site[pat].connecIND[newpat]].F[i][j] += randomCounts[newpat];
						}
					}
				}
			}
		}
	}
}

void LayEggs() {
	double num;
	std::vector<double> larv(TL);
	std::vector<int> randomCounts;
	for (int i=0; i<TL; i++) {
		larv[i] = pa.LarvProbs[i]; // again, is this local variable necessary if already have global variable (or potentially later have this function as method with this variable as attribute, which will have same effect)? use pointers?
	}

	for (int pat=0; pat < Site.size(); pat++) {
		// fraction of 0:ww, 1:wd, 2:dd, 3:wr, 4:rr, 5:dr
		for (int i=0; i<NumGen; i++) {
			for (int j=0; j<NumGen; j++) {
				for (int k=0; k<NumGen; k++) {
					num = Site[pat].F[i][j] * pa.f[i][j][k]; // number of eggs laid with each genotype
					num = random_poisson(pa.theta * num); // normalisation(?) of number of eggs laid according to distribution of number of eggs laid per day

					// num: double -> int in multinomial function!!
					randomCounts = random_Multinomial(num, larv); // number of larvae that actually survive coming out of egg (?)

					for (int a=0; a<TL; a++) {
						Site[pat].J[k][a] += randomCounts[a];
					}
					//	aww = random_multinom_var(num, TL, &pa.LarvProbs[0], 1.0);
					//	for (int a=0; a<TL; a++) Site[pat].J[k][a] += *(aww+a);
					//	delete[] aww;
					to.J[k] += num;
					to.JTot += num;
				}
			}
		}
	}
}

void JuvEmerge() {
	int surv, survM;
	double pcomp;
	for (int pat=0; pat < Site.size(); pat++) {
		pcomp = (1 - pa.muJ) * Site[pat].comp;

		for (int i=0; i<NumGen; i++) {
			surv = random_binomial(Site[pat].J[i][0], pcomp); // number of juveniles that survive eclosion (?)

			if (surv > 0) {		
				survM = random_binomial(surv, 0.5); // roughly half of the juveniles become male and half female following a distribution
				Site[pat].MTot += survM;
				Site[pat].M[i] += survM; 
				to.M[i] += survM;
				to.MTot += survM;
				Site[pat].V[i] += surv - survM;
				to.V[i] += surv - survM;
				to.VTot += surv - survM;
			}
		}
	}
}

void Hide() {
	long long int num;
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++) {
			for (int j=0; j<NumGen; j++) {
				num = random_binomial(Site[pat].F[i][j], pa.psi); // number of females that go (or try to go?) into aestivation
				Site[pat].F[i][j] -= num;
				to.F[i] -= num;
				to.FTot -= num;
				Site[pat].AesF[i][j] += random_binomial(num, 1 - pa.muAES);	// number that actually survive going into aestivation
			}
		}
	}
}

void Wake(int day) {
	long long int num;
	double prob = 1.0 / (1.0 + pa.t_wake2 - (day%365)); // probability of a female waking on a given day
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++) {
			for(int j=0; j<NumGen; j++) {
				num = random_binomial(Site[pat].AesF[i][j], prob); // number of females that wake up from aestivation on the given day
				Site[pat].F[i][j] += num;
				to.F[i] += num;
				to.FTot += num;
				Site[pat].AesF[i][j] -= num;	
			}
		}
	}
}

void UpdateComp() {
	// for rainfall extension?
	// int week = (int)(day % 365) / 7; // can safely remove this type cast
	// if (week == 52) { // why?
	// 	week = 51;
	// }

	for (int pat = 0; pat < Site.size(); pat++) {
			Site[pat].comp = std::pow(pa.alpha0 / (pa.alpha0 + Site[pat].JTot + 0.0001), 1 / pa.meanTL);
	}
}

void UpdateMate() {
	for (int pat=0; pat < Site.size(); pat++) {
		Site[pat].mate_rate = Site[pat].MTot / (pa.beta + Site[pat].MTot);
	}
}

void SetFertility() {
	// fraction of 0:ww, 1:wd, 2:dd, 3:wr, 4:rr, 5:dr
	double Fwwww[6] = {1, 0, 0, 0, 0, 0};
	double Fwwwd[6] = {(1 - pa.e - pa.gamma) * 0.5, (1 + pa.e) * 0.5, 0, pa.gamma * 0.5, 0, 0};
	double Fwwdd[6] = {0, 1, 0, 0, 0, 0};
	double Fwwwr[6] = {0.5, 0, 0, 0.5, 0, 0};
	double Fwwrr[6] = {0, 0, 0, 1, 0, 0};
	double Fwwdr[6] = {0, 0.5, 0, 0.5, 0, 0};
	double Fwdww[6] = {(1 - pa.xi)*(1 - pa.e - pa.gamma)*0.5, (1 - pa.xi)*(1 + pa.e)*0.5, 0, (1 - pa.xi)*pa.gamma*0.5, 0, 0};
	double Fwdwd[6] = {(1 - pa.xi)*(1 - pa.e - pa.gamma)*(1 - pa.e - pa.gamma)* 0.25, (1 - pa.xi)*(1 - pa.e - pa.gamma)*(1 + pa.e)*0.5, (1 - pa.xi)*(1 + pa.e)*(1 + pa.e)*0.25, (1 - pa.xi)*(1 - pa.e - pa.gamma)*pa.gamma*0.5, (1 - pa.xi)*pa.gamma*pa.gamma*0.25, (1 - pa.xi)*(1 + pa.e)*pa.gamma*0.5};

	double Fwddd[6] = {0, (1 - pa.xi)*(1 - pa.e-pa.gamma)*0.5, (1 - pa.xi)*(1 + pa.e)*0.5, 0, 0, (1-pa.xi)*pa.gamma*0.5};

	double Fwdwr[6] = {(1 - pa.xi)*(1 - pa.e - pa.gamma)*0.25, (1 - pa.xi)*(1 + pa.e)*0.25, 0, (1 - pa.xi)*((1 - pa.e - pa.gamma)*0.25 + (pa.gamma * 0.25)), (1 - pa.xi)*pa.gamma*0.25, (1 - pa.xi)*(1 + pa.e)*0.25};

	double Fwdrr[6] = {0, 0, 0, (1 - pa.xi)*(1 - pa.e - pa.gamma)*0.5, (1 - pa.xi)*pa.gamma*0.5, (1 - pa.xi)*(1 + pa.e)*0.5};
	double Fwddr[6] = {0, (1 - pa.xi)*(1 - pa.e - pa.gamma)*0.25, (1 - pa.xi)*(1 + pa.e)*0.25, (1 - pa.xi)*(1 - pa.e - pa.gamma)*0.25, (1 - pa.xi)*pa.gamma*0.25, (1 - pa.xi)*((1 + pa.e)*0.25 + pa.gamma*0.25)};
	double Fddww[6] = {0, 0, 0, 0, 0, 0};
	double Fddwd[6] = {0, 0, 0, 0, 0, 0};
	double Fdddd[6] = {0, 0, 0, 0, 0, 0};
	double Fddwr[6] = {0, 0, 0, 0, 0, 0};
	double Fddrr[6] = {0, 0, 0, 0, 0, 0};
	double Fdddr[6] = {0, 0, 0, 0, 0, 0};
	double Fwrww[6] = {0.5, 0, 0, 0.5, 0};
	double Fwrwd[6] = {(1 - pa.e - pa.gamma)*0.25, (1 + pa.e)*0.25, 0, (pa.gamma * 0.25 + (1 - pa.e - pa.gamma) * 0.25), pa.gamma*0.25, (1 + pa.e)*0.25};
	double Fwrdd[6] = {0, 0.5, 0, 0, 0, 0.5};
	double Fwrwr[6] = {0.25, 0, 0, 0.5, 0.25, 0};
	double Fwrrr[6] = {0, 0, 0, 0.5, 0.5, 0};
	double Fwrdr[6] = {0, 0.25, 0, 0.25, 0.25, 0.25};
	double Frrww[6] = {0, 0, 0, 0, 0, 0};
	double Frrwd[6] = {0, 0, 0, 0, 0, 0};
	double Frrdd[6] = {0, 0, 0, 0, 0, 0};
	double Frrwr[6] = {0, 0, 0, 0, 0, 0};
	double Frrrr[6] = {0, 0, 0, 0, 0, 0};
	double Frrdr[6] = {0, 0, 0, 0, 0, 0};

	double Fdrww[6] = {0, 0, 0, 0, 0, 0};
	double Fdrwd[6] = {0, 0, 0, 0, 0, 0};
	double Fdrdd[6] = {0, 0, 0, 0, 0, 0};
	double Fdrwr[6] = {0, 0, 0, 0, 0, 0};
	double Fdrrr[6] = {0, 0, 0, 0, 0, 0};
	double Fdrdr[6] = {0, 0, 0, 0, 0, 0};

	// create 3d array of F_{ijk} to simplify if statements? and once created and assigned values, delete all other 1d arrays from memory

	for (int k=0; k<6; k++) {
		for (int i=0; i<6; i++) {
			for (int j=0; j<6; j++) {
				// potentially substitute this logic for switch statements
				if (i==0) {
					if (j==0) pa.f[i][j][k] = Fwwww[k];
					if (j==1) pa.f[i][j][k] = Fwwwd[k];
					if (j==2) pa.f[i][j][k] = Fwwdd[k];
					if (j==3) pa.f[i][j][k] = Fwwwr[k];
					if (j==4) pa.f[i][j][k] = Fwwrr[k];
					if (j==5) pa.f[i][j][k] = Fwwdr[k];
				}
				if (i==1) {
					if (j==0) pa.f[i][j][k] = Fwdww[k];
					if (j==1) pa.f[i][j][k] = Fwdwd[k];
					if (j==2) pa.f[i][j][k] = Fwddd[k];
					if (j==3) pa.f[i][j][k] = Fwdwr[k];
					if (j==4) pa.f[i][j][k] = Fwdrr[k];
					if (j==5) pa.f[i][j][k] = Fwddr[k];
				}
				if (i==2) {
					if (j==0) pa.f[i][j][k] = Fddww[k];
					if (j==1) pa.f[i][j][k] = Fddwd[k];
					if (j==2) pa.f[i][j][k] = Fdddd[k];
					if (j==3) pa.f[i][j][k] = Fddwr[k];
					if (j==4) pa.f[i][j][k] = Fddrr[k];
					if (j==5) pa.f[i][j][k] = Fdddr[k];
				}
				if (i==3) {
					if (j==0) pa.f[i][j][k] = Fwrww[k];
					if (j==1) pa.f[i][j][k] = Fwrwd[k];
					if (j==2) pa.f[i][j][k] = Fwrdd[k];
					if (j==3) pa.f[i][j][k] = Fwrwr[k];
					if (j==4) pa.f[i][j][k] = Fwrrr[k];
					if (j==5) pa.f[i][j][k] = Fwrdr[k];
				}
				if (i==4) {
					if (j==0) pa.f[i][j][k] = Frrww[k];
					if (j==1) pa.f[i][j][k] = Frrwd[k];
					if (j==2) pa.f[i][j][k] = Frrdd[k];
					if (j==3) pa.f[i][j][k] = Frrwr[k];
					if (j==4) pa.f[i][j][k] = Frrrr[k];
					if (j==5) pa.f[i][j][k] = Frrdr[k];
				}
				if (i==5) {
					if (j==0) pa.f[i][j][k] = Fdrww[k];
					if (j==1) pa.f[i][j][k] = Fdrwd[k];
					if (j==2) pa.f[i][j][k] = Fdrdd[k];
					if (j==3) pa.f[i][j][k] = Fdrwr[k];
					if (j==4) pa.f[i][j][k] = Fdrrr[k];
					if (j==5) pa.f[i][j][k] = Fdrdr[k];
				}
			}
		}
	}	
}

// periodic distance function
double distance (double U, double x1, double y1, double x2, double y2) {
	double xdist = 0;
	double ydist = 0;

	// check logic of this function
	if (abso(x1 - x2) > U - abso(x1 - x2)) {
		xdist = U - abso(x1 - x2);
	} 
	else if (abso(x1 - x2) <= U - abso(x1 - x2)) {
		xdist = abso(x1 - x2);
	}
	// same here!
	if (abso(y1 - y2) > U - abso(y1 - y2)) {
		ydist = U - abso(y1 - y2);
	}
	else if (abso(y1 - y2) <= U - abso(y1 - y2)) {
		ydist = abso(y1 - y2);
	}

	return double(std::sqrt((xdist * xdist) + (ydist * ydist)));
}

double abso(double XX) {
	double YY = XX;
	if (XX < 0) {
		YY *= -1;
	}
	return YY;
}

// Returns a random floating-point number from a uniform real distribution of 0.0 to 1.0
double Random() {
	std::uniform_real_distribution<> dist(0.0, 1.0);
	return dist(gen);
}

// Returns a random integer number from a uniform discrete distribution of a to b
int IRandom(int a, int b) {
	std::uniform_int_distribution<> dist(a, b);
	return dist(gen);
}

long long int random_poisson(double landa) {
	if (landa < 1e-5) { 
		return 0;
	}
	else if (landa > 30) {
		// use normal approximation	
		std::normal_distribution<> dist(landa, std::sqrt(landa)); // dist(mean, standard deviation)
		int x = std::round(dist(gen));
		return std::max(0, x);     // shouldn't have two return statements here!!! find out what should actually be returned, if want both or only one
	}
	else {
		// sample poisson directly
		std::poisson_distribution<> dist(landa); // dist(mean)
		return dist(gen);
	}
}

long long int random_binomial(long long int N, double p) {
	if (N * p > 10 && N * (1 - p) > 10) { // N * p casts p to int (check this)
		// use normal approximation
		std::normal_distribution<> dist(N * p, std::sqrt(N * p * (1 - p))); // dist(mean, standard deviation)
		int x = std::round(dist(gen));
		if (x<0) x=0;
		if (x>N) x=N;

		return x;
	}
	else if ((N > 20 && p < 0.05) || (N > 100 && N*p < 10)) {
		// use Poisson approximation
		return random_poisson(N*p);
	}
	else if ((N > 20 && p > 0.95) || (N > 100 && N*(1-p) < 10)) {
		// use Poisson approximation
		return N - random_poisson(N * (1 - p));
	}
	else {
		// use binomial distribution directly
		std::binomial_distribution<> dist(N, p);
		return dist(gen);
	}
}

std::vector<int> random_Multinomial(int N, const std::vector<double>& probs) {
	int num_outcomes = probs.size();
	std::vector<int> result(num_outcomes, 0);
	std::vector<double> scaled_probabilities(num_outcomes, 0.0);

	// find sum of outcome probabilities
	double sum_p = 0.0;
	for (int i = 0; i < num_outcomes; ++i) {
		sum_p += probs[i];
	}

	int Nused = N;
	for(int pat=0; pat < num_outcomes; pat++) { // rename pat index, not necessarily relating to patches
		if (Nused > 0) {
			result[pat] = random_binomial(Nused, probs[pat] / sum_p);
			sum_p -= probs[pat];
			Nused -= result[pat];
		}
		else {
			result[pat] = 0;
		}
	}

	return result;
}

void CheckCounts(int TT, char ref) {
	int totF = 0, totM = 0, totJ = 0, totV = 0;
	int totFB = 0, totMB = 0, totJB = 0, totVB = 0; // what is the difference? these are the model parameters and others are manually calculated in this function?
	
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++){
			for (int j=0; j < NumGen; j++) {
				if (Site[pat].F[i][j] < 0) {
					std::cout << TT << "\t" << ref << " F neg  " << i << "\t" << j << "\t" << Site[pat].F[i][j] << std::endl;
					std::exit(1);
				}
			}
		}

		for (int i=0; i<NumGen; i++) {
			totF += std::accumulate(Site[pat].F[i], Site[pat].F[i] + NumGen, 0); // ?? why add NumGen?
		}

		totM += std::accumulate(Site[pat].M, Site[pat].M + NumGen, 0);
		totV += std::accumulate(Site[pat].V, Site[pat].V + NumGen, 0);
		for (int i=0; i<NumGen; i++) {
			for (int a=0; a<TL; a++) {
				totJ += Site[pat].J[i][a];
			}
		}
	}

	totFB += std::accumulate(to.F, to.F + NumGen, 0); // ?? why add NumGen?
	totMB += std::accumulate(to.M, to.M + NumGen, 0);
	totVB += std::accumulate(to.V, to.V + NumGen, 0);
	totJB += std::accumulate(to.J, to.J + NumGen, 0);

	if (totF != to.FTot) {
		std::cout << TT << "\t" << ref << " F count error  " << totF << "\t" << totFB << "\t" << to.FTot << std::endl;
		std::exit(1);
	}
	if (totM != to.MTot) {
		std::cout << TT << "\t" << ref << " M count error  " << totM << "\t" << totMB << "\t" << to.MTot << std::endl;
		std::exit(1);
	}
	if (totV != to.VTot) {
		std::cout << TT << "\t" << ref << " V count error  " << totV << "\t" << totVB << "\t" << to.VTot << std::endl;
		std::exit(1);
	}
	if (totJ != to.JTot) {
		std::cout << TT << "\t" << ref << " J count error  " << totJ << "\t" << totJB << "\t" << to.JTot << std::endl;
		std::exit(1);
	}
	if (totFB != to.FTot) {
		std::cout << TT << "\t" << ref << " F count errorB  " << totFB << "\t" << to.FTot << std::endl;
		std::exit(1);
	}
	if (totMB != to.MTot) {
		std::cout << TT << "\t" << ref << " M count errorB  " << totMB << "\t" << to.MTot << std::endl;
		std::exit(1);
	}
	if (totVB != to.VTot) {
		std::cout << TT << "\t" << ref << " V count errorB  " << totVB << "\t" << to.VTot << std::endl;
		std::exit(1);
	}
	if (totJB != to.JTot) {
		std::cout << TT << "\t" << ref << " J count errorB  " << totJB << "\t" << to.JTot << std::endl;
		std::exit(1);
	}
}