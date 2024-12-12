
.. _program_listing_file__github_workspace_includes_InputParams.h:

Program Listing for File InputParams.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file__github_workspace_includes_InputParams.h>` (``/github/workspace/includes/InputParams.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_INPUTPARAMS_H
   #define GENERALMETAPOP_INPUTPARAMS_H
   
   struct InputParams {
       int num_runs; 
       int max_t; 
       int num_pat; 
       double mu_j; 
       double mu_a; 
       double beta; 
       double theta; 
       double comp_power; 
       int min_dev; 
       double gamma; 
       double xi; 
       double e; 
       int driver_start; 
       int num_driver_M; 
       int num_driver_sites; 
       double disp_rate; 
       double max_disp; 
       double psi; 
       double mu_aes; 
       int t_hide1; 
       int t_hide2; 
       int t_wake1; 
       int t_wake2; 
       double alpha0_mean; 
       double alpha0_variance; 
       double alpha1; 
       double amp; 
       double resp; 
       int initial_WJ = 10000; 
       int initial_WM = 50000; 
       int initial_WV = 10000; 
       int initial_WF = 40000; 
       int rec_start; 
       int rec_end; 
       int rec_interval_global; 
       int rec_interval_local; 
       int rec_sites_freq; 
       int set_label; 
   };
   
   #endif //GENERALMETAPOP_INPUTPARAMS_H
