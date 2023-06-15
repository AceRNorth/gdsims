#include "headGen.h"

/*--------------------------------------global variables----------------------------------*/
/*----------------------------------------------------------------------------------------*/
/*-------------random number seed-----------------*/
std::random_device rd;
std::mt19937 gen(rd());
/*------------------------------------------------*/
/*----------------global structs------------------*/
Pars pa;//parameters 
initials in; //initial conditions
Times ti; // timekeeping parameters
totals to; // totals
std::vector<Patch> Site; // information on each population
/*------------------------------------------------*/
/*------------------Output files------------------*/
std::ostringstream os1, os2, os3, os4;
std::ofstream globalinfo, localinfo, duration, ParList;
/*------------------------------------------------*/

/*----------------------------------------------------------------------------------------*/
//	clock_t now=clock();


int main()
{
	/*---------input parameters---------------------*/
	std::cin >> pa.set; //enumeration for output files
	std::cin >> pa.index;
	std::cin >> pa.NumPat;// number of sites
	std::cin >> in.recSitesFreq;//if outputting local data, how many sites to collect for (1 is all sites, 10 is 1 in 10 etc)
	std::cin >> ti.interval;//time interval for outputting global data
	std::cin >> ti.start;
	std::cin >> ti.maxT;// maximum time to simulate until (days)
	std::cin >> ti.recfreq;// how often to collect local data (every rec days)
	std::cin >> ti.recstart;// how often to collect local data (every rec days)
	std::cin >> ti.recend;// how often to collect local data (every rec days)
	std::cin >> ti.NumRuns;// how many simulation runs
	std::cin >> in.driver_time;// time to start releasing drive alleles; if negative, the release dates are random, if positive, the releases are on the specific date
	std::cin >> in.driver_start;// time to start releasing drive alleles
	std::cin >> in.driver_end;// time to end releasing drive alleles
	std::cin >> in.NumDriver;// number of drive homozygous male mosquitoes per release
	std::cin >> in.NumDriverSites;// number of release sites per year
	std::cin >> pa.muJ;//juvenile density independent mortality per day
	std::cin >> pa.muA;//adult mortality per day
	std::cin >> pa.d;//adult dispersal rate
	std::cin >> pa.gamma;//rate of r2 allele formation from W/D meiosis
	std::cin >> pa.beta;//parameter that controls mating rate
	std::cin >> pa.theta;//egg laying rate of wildtype females (eggs per day)
	std::cin >> pa.xi;// somatic Cas9 expression fitness cost
	std::cin >> pa.e;// homing rate in females
	std::cin >> pa.LD;//maximum distance at which settlements are connected

	std::cin >> pa.psi;//Aestivation rate
	std::cin >> pa.muAES;//aestivation mortality
	std::cin >> pa.t_hide1;//start day of going into aestivation
	std::cin >> pa.t_hide2;//end day of going into aestivation
	std::cin >> pa.t_wake1;//start day of emerging from aestivation
	std::cin >> pa.t_wake2;//end day of emerging from aestivation
	std::cin >> pa.alpha0;//baseline contribution to carrying capacity
	std::cin >> pa.meanTL;

	for (int x=0; x<TL; x++) {
		std::cin >> pa.LarvProbs[x]; 
	}

	std::cin >> pa.U;
	std::cin >> pa.CentRad;
	// std::cout << pa.U << "   " << pa.muA << "  " << pa.muJ << std::endl;


	/*---------------------------------------------------------------------------------------------------*/
	/*-------------------------------numbers to use when initiating populations--------------------------*/
	int inV = 10000;
	int inM = 50000;
	int inF = 40000;

	for (int a=0; a<TL; a++) {
		in.NumJW[a] = 10000;
	}

	in.NumAdultsWV = inV, in.NumAdultsWM = inM, in.NumAdultsWF = inF;
	/*---------------------------------------------------------------------------------------------------*/


	/*--------------------------------Set inheritance architecture---------------------------------------*/
	SetFertility();
	/*---------------------------------------------------------------------------------------------------*/

	/*--------------------------------run model NumRuns times--------------------------------------------*/
	RunNReps(ti.NumRuns);
	/*---------------------------------------------------------------------------------------------------*/
	return 0;
}
	

void RunNReps(int N) {

	os3 << "CoordinateList" << pa.set << "run" << pa.index << ".txt";
	ParList.open(os3.str().c_str());
	os1 << "LocalData" << pa.set << "run" << pa.index << ".txt";// make a file for outputing local data
	os2 << "Totals" << pa.set << "run" << pa.index << ".txt";// make a file for outputing global data
	localinfo.open(os1.str().c_str());
	globalinfo.open(os2.str().c_str());
	std::clock_t start;
	double dtime;
	start = std::clock();

	for (int j=0; j<N; j++) {
		initiate();

		for (int pat=0; pat < Site.size(); pat += in.recSitesFreq) {
			ParList << Site[pat].x << "   " << Site[pat].y << std::endl;
		}

		RunMaxT();
	}

	dtime = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	os2.str("");
	os1.str("");
	globalinfo.close();
	localinfo.close();
	os3.str("");
	ParList.close();
}

void RunMaxT() {

	int TT = ti.start;
	int uniquepat, relpat, year2, num_release_sites, num_driver;
	
	/*-----------------set gene drive release sites------------------*/
	num_release_sites = min(to.CentSqVils, int(in.NumDriverSites));
	int relpatches[num_release_sites];
	int reltimes[num_release_sites];
	
	for (int jj=0; jj < num_release_sites; jj++) {
		relpatches[jj] = -1; //initialise elements outside the range of possible values, to prevent interference
	}

	for (int jj=0; jj < num_release_sites; jj++) {
		uniquepat = 0;

		while (uniquepat == 0) {
			uniquepat = 1;
			relpat = IRandom(0, Site.size() - 1);

			for (int ii=0; ii < num_release_sites; ii++) {
				if (relpat == relpatches[ii] || Site[relpat].CentSq == 0) {
					uniquepat=0;
				}
			}
			relpatches[jj] = relpat;
		}

		reltimes[jj] = in.driver_time;
	}
	/*------------------------------------------------------------*/
	while (TT < ti.maxT + 1) {
		/*----------outputting------------------------*/	
		if (TT % ti.interval == 0) {
			std::cout << TT << "   " << to.JTot << "   " << to.MTot << "    " << to.VTot << "   " << to.FTot << std::endl;
			globalinfo << TT;
			
			for (int i=0; i<NumGen; i++) {
				globalinfo << "     " << to.M[i];
			}

			globalinfo << std::endl;
		}

		if (TT > ti.recstart && TT <= ti.recend && TT%ti.recfreq == 0) {
			record(TT);
		}
		/*------------------------------------------------*/
		for (int jj=0; jj < num_release_sites; jj++) {
			if (TT == reltimes[jj]){
				//gene drive release
				PutDriverSites(relpatches[jj]);
			}
		}

		OneStep(TT);
		TT++;
	}
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
	to.CentSqHum = 0;
	Site.clear();
	Patch pp;
	//cout << "init   1" << endl;
	/*----------------------------------------------------------------------------------*/
	/*------------------------------input the settlement data---------------------------*/
	for (int ii=0; ii < pa.NumPat; ii++) {       
		pp.x = Random() * pa.U;
		pp.y = Random() * pa.U;
		pp.sqx = 0;
		pp.sqy = 0;
		pp.CentSq = 0;

		if (distance(pa.U, pp.x, pp.y, pa.U / 2.0, pa.U / 2.0) < pa.CentRad) {
			pp.CentSq = 1;
		}

		pp.gam = 0;
		pp.arab = 0;
		pp.fun = 0;
		pp.area = 1;
		pp.connecIND.clear();
		pp.connecW.clear();
		pp.TotW = 0;

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
	/*----------------------------------------------------------------------------------*/

	for (int i=0; i < Site.size(); i++) {
		if (Site[i].CentSq == 1) {
			SitesPopulate(i);
		}
	}

	UpdateConnec();
//		CheckCounts(0,'i');
//cout<<"finished initialising"<<endl;
}


void UpdateConnec() {
	double dd, ww, cor;
	for (int index = 0; index < Site.size(); index++) {
		Site[index].connecIND.clear();
		Site[index].connecW.clear();
		Site[index].TotW = 0;
		for (int ii=0; ii < Site.size(); ii++) {
		// re-activate this if loop to exclude self-dispersal. need to check isolated sites catered for
		//	if (ii != index) {
				dd = distance(pa.U, Site[index].x, Site[index].y, Site[ii].x, Site[ii].y);
				if (dd < pa.LD) {
					Site[index].connecIND.push_back(ii); 
					//ww = (1 - dd / pa.LD) * (3 / (PI * pa.LD * pa.LD)) / (1.0 * pa.NumPat);
					ww = pa.LD-dd;
					Site[index].connecW.push_back(ww); 
					Site[index].TotW += ww;
				}
		//	}
		}
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
	//	
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
	UpdateComp(day);
	UpdateMate();
}
	

void JuvEmerge() {
	int surv, survM;
	double pcomp;
	for (int pat=0; pat < Site.size(); pat++) {
		pcomp = (1 - pa.muJ) * Site[pat].comp;

		for (int i=0; i<NumGen; i++) {
			surv = random_binomial(Site[pat].J[i][0], pcomp); 

			if (surv > 0) {		
				survM = random_binomial(surv, 0.5);
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

		for (int age=0; age < TL - 1; age++) {
			for (int i=0; i<NumGen; i++) {		
				Site[pat].J[i][age] = random_binomial(Site[pat].J[i][age+1], pcomp); // increment age by 1 taking mortality into account (pcomp is survival per larva)
				jtSitesch += Site[pat].J[i][age];
				jtAll[i] += Site[pat].J[i][age];
			}
		}
		
		Site[pat].JTot = jtSitesch;
		jtAllAll += jtSitesch;
		for (int i=0; i<NumGen; i++) {
			Site[pat].J[i][TL-1] = 0;
		} 
	}

	for (int i=0; i<NumGen; i++) {
		to.J[i] = jtAll[i];
	}

	to.JTot = jtAllAll;
}

void VirginsMate() {
	int v[NumGen];
	int f[NumGen];
	std::vector<double> m(NumGen);
	std::vector<int> randomCounts;
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++) {
			m[i] = (1.0 * Site[pat].M[i]);
		}

		for (int i=0; i<NumGen; i++) {
			v[i] = random_binomial(Site[pat].V[i], Site[pat].mate_rate); // how many V will mate

			if (v[i] > 0) {
				randomCounts = random_Multinomial(v[i], m);

				for (int j=0; j<NumGen; j++) {
					Site[pat].F[i][j] += randomCounts[j];
				}

				to.F[i] += v[i];
				Site[pat].V[i] -= v[i];
				to.V[i] -= v[i];
				to.VTot -= v[i];
				to.FTot += v[i];
			}
		}
	}
}

void AdultsMove() {
	int pat, newpat;
	std::vector<int> randomCounts;
	if (Site.size() > 1) {
		for (pat=0; pat < Site.size(); pat++) {
			for (int i=0; i<NumGen; i++) {
				Site[pat].MoveM[i] = random_binomial(Site[pat].M[i], pa.d);
				Site[pat].M[i] -= Site[pat].MoveM[i];
				Site[pat].MTot -= Site[pat].MoveM[i];

				for (int j=0; j<NumGen; j++) {
					Site[pat].MoveF[i][j] = random_binomial(Site[pat].F[i][j], pa.d);
					Site[pat].F[i][j] -= Site[pat].MoveF[i][j]; 
				}
			}
		}

		for (pat=0; pat < Site.size(); pat++) {
			for (int i=0; i<NumGen; i++) {
				randomCounts = random_Multinomial(Site[pat].MoveM[i], Site[pat].connecW);

				for (newpat=0; newpat < randomCounts.size(); newpat++) {
					Site[Site[pat].connecIND[newpat]].M[i] += randomCounts[newpat];
				}

				for (int j=0; j<NumGen; j++) {
					if (Site[pat].MoveF[i][j] > 0) {
						//std::vector<int> randomCounts = random_Multinomial(Site[pat].MoveF[i][j], Site[pat].connecW);
						randomCounts = random_Multinomial(Site[pat].MoveF[i][j], Site[pat].connecW);

						for (newpat=0; newpat < randomCounts.size(); newpat++) {
							Site[Site[pat].connecIND[newpat]].F[i][j] += randomCounts[newpat];
						}
					}
				}
			}
		}
	}
}

void Hide() {
	long long int num;
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++) {
			for (int j=0; j<NumGen; j++) {
				num = random_binomial(Site[pat].F[i][j], pa.psi);
				Site[pat].F[i][j] -= num;
				to.F[i] -= num;
				to.FTot -= num;
				Site[pat].AesF[i][j] += random_binomial(num, 1 - pa.muAES);	
			}
		}
	}
}

void Wake(int day) {
	long long int num;
	double prob = 1.0 / (1.0 + pa.t_wake2 - (day%365));
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++) {
			for(int j=0; j<NumGen; j++) {
				num = random_binomial(Site[pat].AesF[i][j], prob);
				Site[pat].F[i][j] += num;
				to.F[i] += num;
				to.FTot += num;
				Site[pat].AesF[i][j] -= num;	
			}
		}
	}
}

void LayEggs() {
	double num;
	std::vector<double> larv(TL);
	std::vector<int> randomCounts;
	for (int i=0; i<TL; i++) {
		larv[i] = pa.LarvProbs[i];
	}

	int* aww;
	for (int pat=0; pat < Site.size(); pat++) {
		// fraction of 0:ww,1:wd,2:dd,3:wr,4:rr,5:dr
		for (int i=0; i<NumGen; i++) {
			for (int j=0; j<NumGen; j++) {
				for (int k=0; k<NumGen; k++) {
					num = Site[pat].F[i][j] * pa.f[i][j][k];
					num = random_poisson(pa.theta * num);

					randomCounts = random_Multinomial(num, larv);

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

void UpdateComp(int day) {
	int week = (int)(day % 365) / 7;
	if (week == 52) {
		week = 51;
	}

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
	//fraction of 0:ww, 1:wd, 2:dd, 3:wr, 4:rr, 5:dr
	double Fwwww[6] = {1, 0, 0, 0, 0, 0};
	double Fwwwd[6] = {(1 - pa.e - pa.gamma) * 0.5, (1 + pa.e) * 0.5, 0, pa.gamma * 0.5, 0, 0};
	double Fwwdd[6] = {0, 1, 0, 0, 0, 0};
	double Fwwwr[6] = {0.5, 0, 0, 0.5, 0, 0};
	double Fwwrr[6] = {0, 0, 0, 1, 0, 0};
	double Fwwdr[6] = {0, 0.5, 0, 0.5, 0, 0};
	double Fwdww[6] = {(1 - pa.xi)*(1 - pa.e - pa.gamma)*0.5, (1 - pa.xi)*(1 + pa.e)*0.5, 0, (1 -pa.xi)*pa.gamma*0.5, 0, 0};
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

double distance (double U, double x1, double y1, double x2, double y2) {
	//periodic distance function
	double xdist = 0;
	double ydist = 0;

	// these might actually be else if statements instead. Also case of abso(x1 - x2) == U - abso(x1 - x2) is not included! 
	if (abso(x1 - x2) > U - abso(x1 - x2)) {
		xdist = U - abso(x1-x2);
	} 
	if (abso(x1 - x2) < U - abso(x1 - x2)) {
		xdist = abso(x1 - x2);
	}
	// same here!
	if (abso(y1 - y2) > U - abso(y1 - y2)) {
		ydist = U - abso(y1 - y2);
	}
	if (abso(y1 - y2) < U - abso(y1 - y2)) {
		ydist = abso(y1 - y2);
	}

	return double(sqrt(xdist * xdist + ydist * ydist));
}

double abso(double XX) {
	double YY = XX;
	if (XX < 0) {
		YY *= -1;
	}
	return YY;
}

long long int random_poisson(double landa) {
	if (landa < 1e-5) {
		return 0;
	}
	else if (landa > 30) {
		//use normal approximation	
		std::normal_distribution<> dist(landa, std::sqrt(landa));
		int x = std::round(dist(gen));
		return std::max(0,x);     // shouldn't have two return statements here!!! find out what should actually be returned, if want both or only one
		return std::round(dist(gen));
	}
	else {
		//sample poisson directly
		std::poisson_distribution<> dist(landa);
		return dist(gen);
	}
}

long long int random_binomial(long long int N, double p) {
	if (N * p > 10 && N * (1 - p) > 10) {
		// Use normal approximation
		std::normal_distribution<> dist(N*p, std::sqrt(N * p * (1 - p)));
		int x = std::round(dist(gen));
		if (x<0) x=0;
		if (x>N) x=N;

		return x;
	}
	else if ((N > 20 && p < 0.05) || (N > 100 && N*p < 10)) {
		// Use Poisson approximation
		return random_poisson(N*p);
	}
	else if ((N > 20 && p > 0.95) || (N > 100 && N*(1-p) < 10)) {
		// Use Poisson approximation
		return N - random_poisson(N * (1 - p));
	}
	else {
		// Use binomial distribution directly
		std::binomial_distribution<> dist(N, p);
		return dist(gen);
	}
}

std::vector<int> random_Multinomial(int N, const std::vector<double>& probs) {
		int num_outcomes = probs.size();
		std::vector<int> result(num_outcomes, 0);
		std::vector<double> scaled_probabilities(num_outcomes, 0.0);

	double sum_p = 0.0;
	for (int i = 0; i < num_outcomes; ++i) {
		sum_p += probs[i];
	}

	int Nused = N;
	for(int pat=0; pat < num_outcomes; pat++) {
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

int IRandom(int a, int b) {
	std::uniform_int_distribution<> dist(a, b);
	return dist(gen);
}

double Random() {
	std::uniform_real_distribution<> dist(0.0, 1.0);
	return dist(gen);
}

void record(int index) {	
	for(int pat=0; pat < Site.size(); pat += in.recSitesFreq) {
		for(int i=0; i < NumGen - 1; i++) {
			// potentially join lines into one statement
			localinfo << Site[pat].M[i] << "    ";
			localinfo << Site[pat].M[5] << std::endl;
		}
	}

}

void CheckCounts(int TT, char ref) {
	int totF = 0, totM = 0, totJ = 0, totV = 0;
	int totFB = 0, totMB = 0, totJB = 0, totVB = 0;
	
	for (int pat=0; pat < Site.size(); pat++) {
		for (int i=0; i<NumGen; i++){
			for (int j=0; j < NumGen; j++) {
				if (Site[pat].F[i][j] < 0) {
					std::cout << TT << "   " << ref << " F neg  " << i << "   " << j << "  " << Site[pat].F[i][j] << std::endl;
					exit(1);
				}
			}
		}

		for (int i=0; i<NumGen; i++) {
			totF += accumulate(Site[pat].F[i],Site[pat].F[i]+NumGen,0);
		}

		totM += accumulate(Site[pat].M, Site[pat].M + NumGen, 0);
		totV += accumulate(Site[pat].V, Site[pat].V + NumGen, 0);
		for (int i=0; i<NumGen; i++) {
			for (int a=0; a<TL; a++) {
				totJ += Site[pat].J[i][a];
			}
		}
	}

	totFB += accumulate(to.F, to.F + NumGen, 0);
	totMB += accumulate(to.M, to.M + NumGen, 0);
	totVB += accumulate(to.V, to.V + NumGen, 0);
	totJB += accumulate(to.J, to.J + NumGen, 0);

	if (totF != to.FTot) {
		std::cout << TT << "   " << ref << " F count error  " << totF << "   " << totFB << "   " << to.FTot << std::endl;
		exit(1);
	}
	if (totM != to.MTot) {
		std::cout << TT << "   " << ref << " M count error  " << totM << "   " << totMB << "   " << to.MTot << std::endl;
		exit(1);
	}
	if (totV != to.VTot) {
		std::cout << TT << "   " << ref << " V count error  " << totV << "   " << totVB << "   " << to.VTot << std::endl;
		exit(1);
	}
	if (totJ != to.JTot) {
		std::cout << TT << "   " << ref << " J count error  " << totJ << "   " << totJB << "   " << to.JTot << std::endl;
		exit(1);
	}
	if (totFB != to.FTot) {
		std::cout << TT << "   " << ref << " F count errorB  " << totFB << "   " << to.FTot << std::endl;
		exit(1);
	}
	if (totMB != to.MTot) {
		std::cout << TT << "   " << ref << " M count errorB  " << totMB << "   " << to.MTot << std::endl;
		exit(1);
	}
	if (totVB != to.VTot) {
		std::cout << TT << "   " << ref << " V count errorB  " << totVB << "   " << to.VTot << std::endl;
		exit(1);
	}
	if (totJB != to.JTot) {
		std::cout << TT << "   " << ref << " J count errorB  " << totJB << "   " << to.JTot << std::endl;
		exit(1);
	}
}
