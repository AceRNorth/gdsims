#ifndef GENERALMETAPOP_EXCEPTION_H
#define GENERALMETAPOP_EXCEPTION_H

#include <string>

class Exception {
public:
    Exception();
    virtual void message() = 0;
};

class OutOfBoundsException: public Exception {
public:
    OutOfBoundsException(const std::string& param);
    void message();

private:
    std::string par;
};

class InvalidIntervalException: public Exception {
public:
    InvalidIntervalException(const std::string& param1, const std::string& param2);
    void message();

private:
    std::string inter1;
    std::string inter2;
};

#endif //GENERALMETAPOP_EXCEPTION_H