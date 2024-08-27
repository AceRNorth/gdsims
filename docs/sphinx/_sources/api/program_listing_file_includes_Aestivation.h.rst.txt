
.. _program_listing_file_includes_Aestivation.h:

Program Listing for File Aestivation.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_includes_Aestivation.h>` (``includes\Aestivation.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_AESTIVATION_H
   #define GENERALMETAPOP_AESTIVATION_H
   
   #include <vector>
   #include <array>
   #include "Params.h"
   #include "Patch.h"
   #include "constants.h"
   
   class Patch;
   
   class Aestivation {
   public:
       Aestivation(AestivationParams *params, int sites_size);
       void hide(std::vector<Patch*> &sites);
       void wake(int day, std::vector<Patch*> &sites);
       bool is_hide_time(int day);
       bool is_wake_time(int day);
   
   private:
       double psi; 
       double mu_aes; 
       int t_hide1; 
       int t_hide2; 
       int t_wake1; 
       int t_wake2; 
       std::vector<std::array<std::array<long long int, constants::num_gen>, constants::num_gen>> aes_F; 
   };
   
   #endif //GENERALMETAPOP_AESTIVATION_H
