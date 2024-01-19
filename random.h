#include <vector>
#include <array>
#include "constants.h"

using namespace constants;

// Random number generator functions

double random_real();
int random_discrete(int a, int b);
long long int random_poisson(double lambda);
long long int random_binomial(long long int n, double p);
std::vector<long long int> random_multinomial(long long int n, const std::vector<double>& probs);
std::vector<long long int> random_multinomial(long long int n, const std::array<long long int, num_gen>& probs);
std::vector<long long int> random_multinomial(long long int n, const std::array<double, max_dev+1>& probs);