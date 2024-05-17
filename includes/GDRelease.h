#ifndef GENERALMETAPOP_GDRELEASE_H
#define GENERALMETAPOP_GDRELEASE_H

#include <vector>
#include "Params.h"
#include "Patch.h"

class Patch;

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

#endif //GENERALMETAPOP_GDRELEASE_H