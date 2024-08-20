
.. _program_listing_file_includes_Record.h:

Program Listing for File Record.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_includes_Record.h>` (``includes\Record.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_RECORD_H
   #define GENERALMETAPOP_RECORD_H
   
   #include <sstream>
   #include <fstream>
   #include <vector>
   #include <array>
   #include "constants.h"
   #include "Patch.h"
   #include "Params.h"
   
   class Record {
   public:
       Record(RecordParams *rec_params, int rep);
       ~Record();
       void record_coords(const std::vector<Patch*> &sites);
       void record_global(int day, const std::array<long long int, constants::num_gen> &tot_M_gen);
       void output_totals(int day, long long int tot_J, long long int tot_M, long long int tot_V, long long int tot_F);
       void record_local(int day, const std::vector<Patch*> &sites);
   
       bool is_rec_local_time(int day);
       bool is_rec_global_time(int day);
   
   private:
       // recording window and intervals
   
       int rec_start; 
       int rec_end; 
       int rec_interval_global; 
       int rec_interval_local; 
       int rec_sites_freq; 
       // output filename labels
   
       int set_label; 
       int rep_label; 
       std::ostringstream os1; 
       std::ostringstream os2; 
       std::ostringstream os3; 
       std::ofstream local_data; 
       std::ofstream global_data; 
       std::ofstream coord_list; 
   };
   
   #endif //GENERALMETAPOP_RECORD_H
