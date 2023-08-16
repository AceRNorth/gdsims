#include <iostream>
#include <array>

const int max_dev = 20; // juvenile development time (egg to adult) expressed as days left till eclosion (eclosion on day 0)
const int num_gen = 6;

class Patch {
public:
    std::array<std::array<long long int, max_dev>, num_gen> J; 
    std::array<long long int, num_gen> M; 
    std::array<long long int, num_gen> V; 
    std::array<std::array<long long int, num_gen>, num_gen> F;
    long long int tot_J; 
    long long int tot_M; 
    long long int tot_V;
    long long int tot_F;
    friend Patch operator+=(Patch& patch, );
};


int main ()
{
    Patch
    return 0;
}