#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <fstream>
#include <string>
#include <limits> 
#include <sstream>

// Checks if a value falls within bounds. 
template <typename T>
bool check_bounds(const std::string &par_name, T value, T lower_bound, bool inclusive_lower = true,
 T upper_bound = std::numeric_limits<T>::max(), bool inclusive_upper = true) 
{
    if (lower_bound >= upper_bound) {
        std::cerr << "Error: lower_bound must be less than upper_bound." << std::endl;
        return false;
    }

    bool lower_check = inclusive_lower ? value >= lower_bound : value > lower_bound;
    bool upper_check = inclusive_upper ? value <= upper_bound : value < upper_bound;

    std::string lower_bound_char = inclusive_lower ? "\u2264" : "<";
    std::string upper_bound_char = inclusive_upper ? "\u2264" : "<";
    if (!lower_check || !upper_check) {
        std::cout << "The parameter " << par_name << " is out of bounds. ";
        std::cout << par_name << " should be " << lower_bound << " " << lower_bound_char << " " << par_name << " ";
        std::cout <<  upper_bound_char << " " << upper_bound << ". " << std::endl;
    }

    return lower_check && upper_check;
}

// Reads a value from filestream and assigns it to the parameter variable if the types match.
template <typename T>
bool read_and_validate_type(std::ifstream &file, T &par, const std::string &par_name, const std::string &par_type) 
{
	if (!(file >> par)) {
        std::cerr << "Error: invalid type for " << par_name << ". Expected " << par_type << "." << std::endl;
        return false;
    }
    return true;
}

template <typename T>
bool read_and_validate_type(std::stringstream &linestream, T &par, const std::string &par_name, const std::string &par_type)
{
    if (!(linestream >> par)) {
        std::cerr << "Error: invalid type for " << par_name << ". Expected " << par_type << "." << std::endl;
        return false;
    }
    return true;
}

#endif //INPUT_H
