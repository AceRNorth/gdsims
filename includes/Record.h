#ifndef RECORD_H
#define RECORD_H

#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include "constants.h"
#include "Patch.h"
#include "Params.h"

using namespace constants;

// Records model data.
class Record {
public:
	Record(RecordParams *rec_params, int rep);
	~Record();
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

#endif //RECORD_H