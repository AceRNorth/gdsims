#ifndef GENERALMETAPOP_INPUTVAL_H
#define GENERALMETAPOP_INPUTVAL_H

#include <iostream>
#include <fstream>
#include <string>
#include <limits> 
#include <sstream>

/** 
 * Checks if a value falls within bounds.
 * @tparam    T                 type of the checked parameter
 * @param[in] par_name          name of the parameter value
 * @param[in] value             parameter value
 * @param[in] lower_bound       value of the lower bound
 * @param[in] inclusive_lower   whether the lower bound value is included in the bounds
 * @param[in] upper_bound       value of the upper bound
 * @param[in] inclusive_upper   whether the upper bound value is included in the bounds
 * @return Whether the value falls within bounds or not. 
 */ 
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

/** 
 * Checks if two values constitute a valid interval.
 * @tparam T type of the interval values
 * @param[in] start_name    name of the interval start parameter 
 * @param[in] stop_name     name of the interval stop parameter
 * @param[in] start         start value of the interval
 * @param[in] stop          stop value of the interval
 */ 
template <typename T>
bool check_interval(const std::string &start_name, const std::string &stop_name, T start, T stop) {
	if (stop <= start) {
		std::cout << "The parameters " << start_name << ", " << stop_name << " do not constitute a valid interval." << std::endl;
		return false;
	}
	return true;
}

/**
 * Reads the next value from a filestream and assigns it to the parameter variable if the types match.
 * @tparam T type of the parameter value
 * @param[in, out]  file        filestream
 * @param[out]      par         parameter variable
 * @param[in]       par_name    parameter name (for error messages)
 * @param[in]       par_type    parameter type (for error messages)
 * @return Whether the assignment process has been successful. 
 */
template <typename T>
bool file_read_and_validate_type(std::ifstream &file, T &par, const std::string &par_name, const std::string &par_type) 
{
	if (!(file >> par)) {
        std::cerr << "Error: invalid type for " << par_name << ". Expected " << par_type << "." << std::endl;
        return false;
    }
    return true;
}

/**
 * Reads the next value from a linestream and assigns it to the parameter variable if the types match.
 * @tparam T type of the parameter value
 * @param[in, out]  linestream  linestream
 * @param[out]      par         parameter variable
 * @param[in]       par_name    parameter name (for error messages)
 * @param[in]       par_type    parameter type (for error messages)
 * @return Whether the assignment process has been successful. 
 */
template <typename T>
bool read_and_validate_type(std::stringstream &linestream, T &par, const std::string &par_name, const std::string &par_type)
{
    if (!(linestream >> par)) {
        std::cerr << "Error: invalid type for " << par_name << ". Expected " << par_type << "." << std::endl;
        return false;
    }
    return true;
}

#endif //GENERALMETAPOP_INPUTVAL_H
