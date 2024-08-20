
.. _program_listing_file_src_Aestivation.cpp:

Program Listing for File Aestivation.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_Aestivation.cpp>` (``src\Aestivation.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <array>
   #include <vector>
   #include "Aestivation.h"
   #include "random.h"
   #include "constants.h"
   
   Aestivation::Aestivation(AestivationParams* params, int sites_size) 
   {
       psi = params->psi;
       mu_aes = params->mu_aes;
       t_hide1 = params->t_hide1;
       t_hide2 = params->t_hide2;
       t_wake1 = params->t_wake1;
       t_wake2 = params->t_wake2;
   
       aes_F.clear();
   
       std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f;
       for (int i=0; i < constants::num_gen; ++i) {
           for (int j=0; j < constants::num_gen; ++j) {
               f[i][j] = 0;
           }
       }
   
       for (int pat=0; pat < sites_size; ++pat) {
           aes_F.push_back(f);
       }
   }
   
   void Aestivation::hide(std::vector<Patch*> &sites) 
   {
       std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f;
       std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f_try;
       std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f_aes;
       for (int pat=0; pat < sites.size(); ++pat) {
           f = sites[pat]->get_F();
           for (int i=0; i < constants::num_gen; ++i) {
               for (int j=0; j < constants::num_gen; ++j) {
                   f_try[i][j] = random_binomial(f[i][j], psi); // number of females that attempt to go into aestivation
                   f_aes[i][j] = random_binomial(f_try[i][j], 1 - mu_aes); // number that survive going into aestivation
                   aes_F[pat][i][j] += f_aes[i][j]; // move aestivating females into a separate box variable
               }
           }
           sites[pat]->F_hide(f_try); // update population numbers
       }
   }
   
   void Aestivation::wake(int day, std::vector<Patch*> &sites) 
   {
       double prob = 1.0 / (1.0 + t_wake2 - (day%365)); // probability of a female waking on a given day
       
       std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f_w;
       for (int pat=0; pat < sites.size(); ++pat) {
           for (int i=0; i < constants::num_gen; ++i) {
               for(int j=0; j < constants::num_gen; ++j) {
                   // number of females that wake up from aestivation on the given day
                   f_w[i][j] = random_binomial(aes_F[pat][i][j], prob);
                   aes_F[pat][i][j] -= f_w[i][j];
               }
           }
           sites[pat]->F_wake(f_w);
       }
   }
   
   bool Aestivation::is_hide_time(int day) 
   {
       return day%365 > t_hide1 && day%365 <= t_hide2 && psi > 0.00001;
   }
   
   bool Aestivation::is_wake_time(int day) 
   {
       return day%365 > t_wake1 && day%365 <= t_wake2 && psi > 0.00001;
   }
