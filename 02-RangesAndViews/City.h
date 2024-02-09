#pragma once

#include <iostream>
#include <string>

struct City {

    std::string name;
    unsigned population;
    float area;
    unsigned since;
    std::string state;

    friend std::ostream& operator<< (std::ostream& ostr, const City& city);
};

std::ostream& operator<< (std::ostream& ostr, const City& city);
