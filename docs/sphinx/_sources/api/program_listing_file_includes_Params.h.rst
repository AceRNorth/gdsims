
.. _program_listing_file_includes_Params.h:

Program Listing for File Params.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_includes_Params.h>` (``includes\Params.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_PARAMS_H
   #define GENERALMETAPOP_PARAMS_H
   
   #include <vector>
   
   // ** These parameters should not be modified after being passed to the Simulation or Model **
   
   struct ProgressionParams {
       int num_runs; 
       int max_t; 
   };
   
   struct AreaParams {
       int num_pat; 
   };
   
   struct LifeParams {
       double mu_j; 
       double mu_a; 
       double beta; 
       double theta; 
       double comp_power; 
       int min_dev; 
   }; 
   
   struct InheritanceParams {
       double gamma; 
       double xi; 
       double e; 
   };
   
   struct ReleaseParams {
       int num_driver_M; 
       int num_driver_sites; 
       std::vector<int> release_times; 
   };
   
   struct DispersalParams {
       double disp_rate; 
       double max_disp; 
   };
   
   struct AestivationParams {
       double psi; 
       double mu_aes; 
       int t_hide1; 
       int t_hide2; 
       int t_wake1; 
       int t_wake2; 
   };
   
   struct SineRainfallParams {
       double alpha1; 
       double amp; 
   };
   
   struct InputRainfallParams {
       double alpha1; 
       double resp; 
       std::vector<double> rainfall; 
   };
   
   struct InitialPopsParams {
       int initial_WJ; 
       int initial_WM; 
       int initial_WV; 
       int initial_WF; 
   };
   
   struct ModelParams {
       AreaParams *area;
       InitialPopsParams *initial;
       LifeParams *life;
       AestivationParams *aes;
       DispersalParams *disp; 
       ReleaseParams *rel;
   };
   
   struct RecordParams {
       // recording window and intervals
       int rec_start; 
       int rec_end; 
       int rec_interval_global; 
       int rec_interval_local; 
       int rec_sites_freq; 
       // output filename labels
       int set_label; 
   };
   
   #endif //GENERALMETAPOP_PARAMS_H
