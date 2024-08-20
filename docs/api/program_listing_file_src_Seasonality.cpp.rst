
.. _program_listing_file_src_Seasonality.cpp:

Program Listing for File Seasonality.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_Seasonality.cpp>` (``src\Seasonality.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <cmath>
   #include <vector>
   #include "Seasonality.h"
   #include "constants.h"
   
   
   SineRainfall::SineRainfall(SineRainfallParams *params): Seasonality(params->alpha1) 
   {
       amp = params->amp;
   }
   
   // 
   double SineRainfall::alpha(int day, double alpha0)
   {
       double alpha = alpha0 + alpha1*(1 + amp*std::sin(2 * constants::pi * day/365));
       return alpha;
   }
   
   InputRainfall::InputRainfall(InputRainfallParams *params): Seasonality(params->alpha1) 
   {
       resp = params->resp;
   
       if (!((params->rainfall).empty())) {
           rainfall = params->rainfall;
       }
       else {
           for (int i = 0; i < 365; ++i) {
               rainfall.push_back(0);
           }
       }
   }
   
   double InputRainfall::alpha(int day, double alpha0)
   {
       double r_d;
       if (day == 0) {r_d = rainfall.at(0);} // use day1 rainfall value for day0 as well, to set up the environmental conditions
       else {
           if (rainfall.size() == 365) {
               r_d = day <= 365 ? rainfall.at(day - 1) : rainfall.at(((day - 1)%365)); // simulation days start from 1
           }
           else {
               r_d = rainfall.at(day - 1); // simulation days start from 1
           }
       }
       double alpha = alpha0 + alpha1*(1 - std::exp(-resp * r_d));
       return alpha;
   }
