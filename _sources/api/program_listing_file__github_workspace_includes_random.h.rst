
.. _program_listing_file__github_workspace_includes_random.h:

Program Listing for File random.h
=================================

|exhale_lsh| :ref:`Return to documentation for file <file__github_workspace_includes_random.h>` (``/github/workspace/includes/random.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_RANDOM_H
   #define GENERALMETAPOP_RANDOM_H
   
   #include <vector>
   #include <array>
   #include "constants.h"
   
   // Random number generator functions
   
   double random_real();
   int random_discrete(int a, int b);
   long long int random_poisson(double lambda);
   long long int random_binomial(long long int n, double p);
   std::vector<long long int> random_multinomial(long long int n, const std::vector<double>& probs);
   std::vector<long long int> random_multinomial(long long int n, const std::array<long long int, constants::num_gen>& probs);
   std::vector<long long int> random_multinomial(long long int n, const std::array<double, constants::max_dev+1>& probs);
   double random_lognormal(double des_mean, double des_var);
   
   #endif //GENERALMETAPOP_RANDOM_H
