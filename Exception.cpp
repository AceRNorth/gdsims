#include <iostream>
#include <string>
#include "Exception.h"

Exception::Exception() {}

OutOfBoundsException::OutOfBoundsException(const std::string& param) : par(param){}

void OutOfBoundsException::message() 
{
    std::cout << "The parameter " << par << " is out of bounds. Please enter all the input parameters again." << std::endl;
}

InvalidIntervalException::InvalidIntervalException(const std::string& param1, const std::string& param2)
{
    inter1 = param1;
    inter2 = param2;
}

void InvalidIntervalException::message()
{
    std::cout << "The parameters " << inter1 << ", " << inter2 << " do not constitute a valid interval. Please enter all the input parameters again." << std::endl;
}
